// File for rendering the skybox in the scene

#include "archlib.h"

/*
 *  Draw a cube
 */
void Cube(float x,float y,float z , float th,float ph , float D)
{
    // Vertexes
    Point P1[] = { {-1,-1,+1} , {+1,-1,+1} , {+1,+1,+1} , {-1,+1,+1} }; //  Front
    Point P2[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,+1,-1} , {-1,+1,-1} }; //  Back
    Point P3[] = { {+1,-1,+1} , {+1,-1,-1} , {+1,+1,-1} , {+1,+1,+1} }; //  Right
    Point P4[] = { {-1,-1,-1} , {-1,-1,+1} , {-1,+1,+1} , {-1,+1,-1} }; //  Left
    Point P5[] = { {-1,+1,+1} , {+1,+1,+1} , {+1,+1,-1} , {-1,+1,-1} }; //  Top
    Point P6[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,-1,+1} , {-1,-1,+1} }; //  Bottom

    // SEGFAULT IS HERE.
    Transform(x,y,z,D,D,D,th,ph);
    
    glBegin(GL_QUADS);
        for (int k=0;k<4;k++)
        {
            glVertex3f(P1[k].x,P1[k].y,P1[k].z);
        }
        for (int k=0;k<4;k++)
        {
            glVertex3f(P2[k].x,P2[k].y,P2[k].z);
        }
        for (int k=0;k<4;k++)
        {
            glVertex3f(P3[k].x,P3[k].y,P3[k].z);
        }
        for (int k=0;k<4;k++)
        {
            glVertex3f(P4[k].x,P4[k].y,P4[k].z);
        }
        for (int k=0;k<4;k++)
        {
            glVertex3f(P5[k].x,P5[k].y,P5[k].z);
        }
        for (int k=0;k<4;k++)
        {
            glVertex3f(P6[k].x,P6[k].y,P6[k].z);
        }

    glEnd();
    glPopMatrix();
}

int Skybox(double dim) {

    // dim is distance of each face from the origin.
    // Each face has an area of (2*dim)^2

    glColor3f(0.0f,0.0f,1.0f);
    Cube(0,0,0, 0,0, dim);


    return 0;
}