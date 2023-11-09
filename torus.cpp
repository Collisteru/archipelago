#include "archlib.h"

/*
 *  Draw torus
 */
void Torus(float x,float y,float z , float th,float ph , float S,float r)
{
   int i,j;   // Counters
   int N=4*8; // Number of slices

   Transform(x,y,z,S,S,S,th,ph);
   Color(1,1,0);
   //  Loop along ring
   for (i=0;i<N;i++)
   {
      double th0 =  i   *360.0/N;
      double th1 = (i+1)*360.0/N;
      //  Loop around ring
      for (j=0;j<N;j++)
      {
         double ph0 =  j   *360.0/N;
         double ph1 = (j+1)*360.0/N;
         Point P[4] = { {Cos(th1)*(1+r*Cos(ph0)),-Sin(th1)*(1+r*Cos(ph0)),r*Sin(ph0)} ,
                        {Cos(th0)*(1+r*Cos(ph0)),-Sin(th0)*(1+r*Cos(ph0)),r*Sin(ph0)} ,
                        {Cos(th0)*(1+r*Cos(ph1)),-Sin(th0)*(1+r*Cos(ph1)),r*Sin(ph1)} ,
                        {Cos(th1)*(1+r*Cos(ph1)),-Sin(th1)*(1+r*Cos(ph1)),r*Sin(ph1)} };
         Point N[4] = { {Cos(th1)*Cos(ph0) , -Sin(th1)*Cos(ph0) , Sin(ph0)} ,
                        {Cos(th0)*Cos(ph0) , -Sin(th0)*Cos(ph0) , Sin(ph0)} ,
                        {Cos(th0)*Cos(ph1) , -Sin(th0)*Cos(ph1) , Sin(ph1)} ,
                        {Cos(th1)*Cos(ph1) , -Sin(th1)*Cos(ph1) , Sin(ph1)} };
         Point T[4] = { {static_cast<float>(th1/30.0) , static_cast<float>(ph0/180.0) , 0} ,
                        {static_cast<float>(th0/30.0) , static_cast<float>(ph0/180.0) , 0} ,
                        {static_cast<float>(th0/30.0) , static_cast<float>(ph0/30.0) , 0} ,
                        {static_cast<float>(th1/30.0) , static_cast<float>(ph0/30.0) , 0} };
         DrawPoly(P,N,T,4);
      }
   }
   glPopMatrix();
}