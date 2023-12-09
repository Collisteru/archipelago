// File for rendering the skybox in the scene

#include "archlib.h"
#include <string>

using namespace std;

/*
 *  Draw a cube
 */
void Cube(float x,float y,float z , float th,float ph , float D, unsigned int texturemap[])
{
    // Vertexes
    Point P1[] = { {-1,-1,+1} , {+1,-1,+1} , {+1,+1,+1} , {-1,+1,+1} }; //  Front
    Point P2[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,+1,-1} , {-1,+1,-1} }; //  Back
    Point P3[] = { {+1,-1,+1} , {+1,-1,-1} , {+1,+1,-1} , {+1,+1,+1} }; //  Right
    Point P4[] = { {-1,-1,-1} , {-1,-1,+1} , {-1,+1,+1} , {-1,+1,-1} }; //  Left
    Point P5[] = { {-1,+1,+1} , {+1,+1,+1} , {+1,+1,-1} , {-1,+1,-1} }; //  Top
    Point P6[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,-1,+1} , {-1,-1,+1} }; //  Bottom
    

    // This causes a stack overflow error when not paired with glPopMatrix
    
    // This is hurting textures!
    Transform(x,y,z,D,D,D,th,ph);

    // Two major versions: with texture and without texture
    
    // Without texture
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

    // Set up Skybox textures
    unsigned int skyboxtextures[6];

    // glGenTextures(6, &skyboxtextures);

    // Load textures

    // Set Textures
    skyboxtextures[0] = LoadTexBMP("box_front.bmp");
    skyboxtextures[1] = LoadTexBMP("box_back.bmp");
    skyboxtextures[2] = LoadTexBMP("box_left.bmp");
    skyboxtextures[3] = LoadTexBMP("box_right.bmp");
    skyboxtextures[4] = LoadTexBMP("box_top.bmp");
    skyboxtextures[5] = LoadTexBMP("box_bottom.bmp");

    // dim is distance of each face from the origin.
    // Each face has an area of (2*dim)^2
    
    Point P1[] = { {-1,-1,+1} , {+1,-1,+1} , {+1,+1,+1} , {-1,+1,+1} }; //  Front /
    Point P2[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,+1,-1} , {-1,+1,-1} }; //  Back /
    Point P3[] = { {+1,-1,-1} , {+1,-1,+1} , {+1,+1,+1} , {+1,+1,-1} }; //  Left /
    Point P4[] = { {-1,-1,-1} , {-1,-1,+1} , {-1,+1,+1} , {-1,+1,-1} }; //  Right /
    Point P5[] = { {-1,+0.995,-1} , {+1,+0.995,-1} , {+1,+0.995,+1} , {-1,+0.995,+1} }; //  Top  (We lower the top panel slightly to prevent the seam from showing in the sky)
    Point P6[] = { {-1,-1,-1} , {+1,-1,-1} , {+1,-1,+1} , {-1,-1,+1} }; //  Bottom
    

    Point T1[] = { {1,0} , {0,0} , {0,1} , {1,1} }; // Front / Correct
    Point T2[] = { {0,0} , {1,0} , {1,1} , {0,1} }; // Back / Correct
    Point T3[] = { {0,0} , {1,0} , {1,1} , {0,1} }; // Left / Correct
    Point T4[] = { {1,0} , {0,0} , {0,1} , {1,1} }; // Right 
    Point T5[] = { {1,1} , {0,1} ,  {0,0} , {1,0}}; // Top
    Point T6[] = { {0,0} , {1,0} , {1,1} , {0,1} }; // Bottom

    // Draw Skybox

    Transform(0,0,0,dim,dim,dim,0,0);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , mode?GL_REPLACE:GL_MODULATE);
   
   
   // Draw a Quad
   glBindTexture(GL_TEXTURE_2D,skyboxtextures[0]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T1[k].x, T1[k].y); glVertex3f(P1[k].x,P1[k].y,P1[k].z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxtextures[1]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T2[k].x, T2[k].y); glVertex3f(P2[k].x,P2[k].y,P2[k].z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxtextures[2]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T3[k].x, T3[k].y); glVertex3f(P3[k].x,P3[k].y,P3[k].z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxtextures[3]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T4[k].x, T4[k].y); glVertex3f(P4[k].x,P4[k].y,P4[k].z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxtextures[4]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T5[k].x, T5[k].y); glVertex3f(P5[k].x,P5[k].y,P5[k].z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxtextures[5]);
    glBegin(GL_QUADS);
    for (int k=0;k<4;k++)
    {
        glTexCoord2f(T6[k].x, T6[k].y); glVertex3f(P6[k].x,P6[k].y,P6[k].z);
    }
    glEnd();

   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

    return 0;
}