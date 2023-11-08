#include "archlib.h"


/*
 *  Backsolve LU decomposition
 *     M  4x4 matrix
 *     I  Pivot index
 *     Bx,By,Bz,Bw is RHS
 *     Returns renormalized Point
 */
#define M(i,j) (M[4*j+i])
Point Backsolve(double M[16],int I[4],double Bx,double By,double Bz,double Bw)
{
   int    i,j;                  //  Counters
   double x[4];                 //  Solution vector
   Point  X;                    //  Solution Point
   double b[4] = {Bx,By,Bz,Bw}; //  RHS

   //  Backsolve
   for (i=0;i<4;i++)
   {
      x[i] = b[I[i]];
      for (j=0;j<i;j++)
         x[i] -= M(i,j)*x[j];
   }
   for (i=3;i>=0;i--)
   {
      for (j=i+1;j<4;j++)
         x[i] -= M(i,j)*x[j];
      x[i] /= M(i,i);
   }

   //  Renormalize
   if (fabs(x[3])<1e-10) Fatal("Light position W is zero\n");
   X.x = x[0]/x[3];
   X.y = x[1]/x[3];
   X.z = x[2]/x[3];
   return X;
}


/*
 *  Perform Crout LU decomposition of M in place
 *     M  4x4 matrix
 *     I  Pivot index
 *  Calls Fatal if the matrix is singular
 */
#define M(i,j) (M[4*j+i])
void Crout(double M[16],int I[4])
{
   int i,j,k;  // Counters

   //  Initialize index
   for (j=0;j<4;j++)
      I[j] = j;

   //  Pivot matrix to maximize diagonal
   for (j=0;j<3;j++)
   {
      //  Find largest absolute value in this column (J)
      int J=j;
      for (k=j+1;k<4;k++)
         if (fabs(M(k,j)) > fabs(M(J,j))) J = k;
      //  Swap rows if required
      if (j!=J)
      {
         k=I[j]; I[j]=I[J]; I[J]=k;
         for (k=0;k<4;k++)
         {
            double t=M(j,k); M(j,k)=M(J,k); M(J,k)=t;
         }
      }
   }

   //  Perform Crout LU decomposition
   for (j=0;j<4;j++)
   {
      //  Upper triangular matrix
      for (i=j;i<4;i++)
         for (k=0;k<j;k++)
            M(j,i) -= M(k,i)*M(j,k);
      if (fabs(M(j,j))<1e-10) Fatal("Singular transformation matrix\n");
      //  Lower triangular matrix
      for (i=j+1;i<4;i++)
      {
         for (k=0;k<j;k++)
            M(i,j) -= M(k,j)*M(i,k);
         M(i,j) /= M(j,j);
      }
   }
}


/*
 *  Set up transform
 *     Push and apply transformation
 *     Calculate light position in local coordinate system (Lp)
 *     Calculate clipping plane in local coordinate system (Ec,Nc)
 *  Global variables set: Lp,Nc,Ec
 *  Killer fact 1:  You MUST call this to set transforms for objects
 *  Killer fact 2:  You MUST call glPopMatrix to undo the transform
 */
#define M(i,j) (M[4*j+i])
void Transform(float x0,float y0,float z0,
               float Sx,float Sy,float Sz,
               float th,float ph)
{
   double M[16]; // Transformation matrix
   int    I[4];  // Pivot
   double Z;     // Location of clip plane

   //  Save current matrix
   glPushMatrix();

   //  Build transformation matrix and copy into M
   glPushMatrix();
   glLoadIdentity();
   glTranslated(x0,y0,z0);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScaled(Sx,Sy,Sz);
   glGetDoublev(GL_MODELVIEW_MATRIX,M);
   glPopMatrix();

   //  Apply M to existing transformations
   glMultMatrixd(M);

   /*
    *  Determine light position in this coordinate system
    */
   Crout(M,I);
   Lp = Backsolve(M,I,Lpos[0],Lpos[1],Lpos[2],Lpos[3]);

   /*
    *  Determine clipping plane E & N
    *  Use the entire MODELVIEW matrix here
    */
   glGetDoublev(GL_MODELVIEW_MATRIX,M);
   //  Normal is down the Z axis (0,0,1) since +/- doesn't matter here
   //  The normal matrix is the inverse of the transpose of M
   Nc.x = M(2,0);
   Nc.y = M(2,1);
   Nc.z = M(2,2);
   //  Far  clipping plane for Z-fail should be just less than 8*dim
   //  Near clipping plane for Z-pass should be just more than dim/8
   Crout(M,I);
   Z = (mode==5) ? -7.9*dim : -0.13*dim;
   Ec = Backsolve(M,I,0,0,Z,1);
}