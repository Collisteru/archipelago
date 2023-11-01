#include "archlib.h"

/*
 *  Draw a cube
 */
void Cube(float x,float y,float z , float th,float ph , float D)
{
   //  Vertexes
   Point P1[] = { {-1,-1,+1} , {+1,-1,+1} , {+1,+1,+1} , {-1,+1,+1} }; //  Front
   Point P2[] = { {+1,-1,-1} , {-1,-1,-1} , {-1,+1,-1} , {+1,+1,-1} }; //  Back
   Point P3[] = { {+1,-1,+1} , {+1,-1,-1} , {+1,+1,-1} , {+1,+1,+1} }; //  Right
   Point P4[] = { {-1,-1,-1} , {-1,-1,+1} , {-1,+1,+1} , {-1,+1,-1} }; //  Left
   Point P5[] = { {-1,+1,+1} , {+1,+1,+1} , {+1,+1,-1} , {-1,+1,-1} }; //  Top
   Point P6[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,-1,+1} , {-1,-1,+1} }; //  Bottom
   //  Normals
   Point N1[] = { { 0, 0,+1} , { 0, 0,+1} , { 0, 0,+1} , { 0, 0,+1} }; //  Front
   Point N2[] = { { 0, 0,-1} , { 0, 0,-1} , { 0, 0,-1} , { 0, 0,-1} }; //  Back
   Point N3[] = { {+1, 0, 0} , {+1, 0, 0} , {+1, 0, 0} , {+1, 0, 0} }; //  Right
   Point N4[] = { {-1, 0, 0} , {-1, 0, 0} , {-1, 0, 0} , {-1, 0, 0} }; //  Left
   Point N5[] = { { 0,+1, 0} , { 0,+1, 0} , { 0,+1, 0} , { 0,+1, 0} }; //  Top
   Point N6[] = { { 0,-1, 0} , { 0,-1, 0} , { 0,-1, 0} , { 0,-1, 0} }; //  Bottom
   //  Textures
   Point T[] = { {0,0,0} , {1,0,0} , {1,1,0} , {0,1,0} };

   Transform(x,y,z,D,D,D,th,ph);
   Color(1,0,0); DrawPoly(P1,N1,T,4); //  Front
   Color(0,0,1); DrawPoly(P2,N2,T,4); //  Back
   Color(1,1,0); DrawPoly(P3,N3,T,4); //  Right
   Color(0,1,0); DrawPoly(P4,N4,T,4); //  Left
   Color(0,1,1); DrawPoly(P5,N5,T,4); //  Top
   Color(1,0,1); DrawPoly(P6,N6,T,4); //  Bottom
   glPopMatrix();
}