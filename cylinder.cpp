#include "archlib.h"

/*
 *  Draw a cylinder
 */
void Cylinder(float x,float y,float z , float th,float ph , float R,float H)
{
   int i,j;   // Counters
   int N=4*8; // Number of slices (n) TODO: Consider making this a parameter

   Transform(x,y,z,R,R,H,th,ph);
   Color(0,1,1);
   //  Two end caps (fan of triangles)
   for (j=-1;j<=1;j+=2)
      for (i=0;i<N;i++)
      {
         float th0 = j* i   *360.0/N;
         float th1 = j*(i+1)*360.0/N;
         Point P[3] = { {0,0,j} , {Cos(th0),Sin(th0),j} , {Cos(th1),Sin(th1),j} };
         Point N[3] = { {0,0,j} , {       0,       0,j} , {       0,       0,j} };
         Point T[3] = { {0,0,0} , {Cos(th0),Sin(th0),0} , {Cos(th1),Sin(th1),0} };
         DrawPoly(P,N,T,3);
      }
   //  Cylinder Body (strip of quads)
   for (i=0;i<N;i++)
   {
      float th0 =  i   *360.0/N;
      float th1 = (i+1)*360.0/N;
      Point P[4] = { {Cos(th0),Sin(th0),+1} , {Cos(th0),Sin(th0),-1} , {Cos(th1),Sin(th1),-1} , {Cos(th1),Sin(th1),+1} };
      Point N[4] = { {Cos(th0),Sin(th0), 0} , {Cos(th0),Sin(th0), 0} , {Cos(th1),Sin(th1), 0} , {Cos(th1),Sin(th1), 0} };
      Point T[4] = { {       0,th0/90.0, 0} , {       2,th0/90.0, 0} , {       2,th1/90.0, 0} , {       0,th1/90.0, 0} };
      DrawPoly(P,N,T,4);
   }

   glPopMatrix();
}