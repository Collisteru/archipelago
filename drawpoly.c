#include "archlib.h"

/*
 *  Draw polygon or shadow volume
 *    P[] array of vertexes making up the polygon
 *    N[] array of normals (not used with shadows)
 *    T[] array of texture coordinates (not used with shadows)
 *    n   number of vertexes
 *  Killer fact: the order of points MUST be CCW
 */
void DrawPoly(Point P[],Point N[],Point T[],int n)
{
   //  Draw polygon with normals and textures
   glBegin(GL_POLYGON);
   for (int k=0;k<n;k++)
   {
      glNormal3f(N[k].x,N[k].y,N[k].z);
      glTexCoord2f(T[k].x,T[k].y);
      glVertex3f(P[k].x,P[k].y,P[k].z);
   }
   glEnd();
}