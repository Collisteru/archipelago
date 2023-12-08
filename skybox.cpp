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
    
    // Textures
    Point T1[] = { {0,0} , {1,0} , {1,1} , {0,1} }; //  Front
    
    // This causes a stack overflow error when not paired with glPopMatrix
    Transform(x,y,z,D,D,D,th,ph);

    // Two major versions: with texture and without texture
    
    // Without texture
    if (texturemap == 0) {
        glBegin(GL_QUADS);
            for (int k=0;k<4;k++)
            {
                // glTexCoord2f(T1[k].x,T1[k].y); 
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
    } else {
        // With Texture
            
            
            glEnable(GL_TEXTURE_2D);
            glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , mode?GL_REPLACE:GL_MODULATE);

            glBegin(GL_QUADS);
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[0]);
                glTexCoord2f(T1[k].x, T1[k].y); glVertex3f(P1[k].x,P1[k].y,P1[k].z);
            }
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[1]);
                glVertex3f(P2[k].x,P2[k].y,P2[k].z);
            }
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[2]);
                glVertex3f(P3[k].x,P3[k].y,P3[k].z);
            }
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[3]);
                glVertex3f(P4[k].x,P4[k].y,P4[k].z);
            }
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[4]);
                glVertex3f(P5[k].x,P5[k].y,P5[k].z);
            }
            for (int k=0;k<4;k++)
            {
                glBindTexture(GL_TEXTURE_2D, texturemap[5]);
                glVertex3f(P6[k].x,P6[k].y,P6[k].z);
            }
            glEnd();
        
            glDisable(GL_TEXTURE_2D);
    }
    
    glPopMatrix();
}

int Skybox(double dim) {

    // Set up Skybox textures
    unsigned int skyboxtextures[6];

    // glGenTextures(6, &skyboxtextures);

    // Load textures

    // Enable Textures
    glEnable(GL_TEXTURE_2D);


    // Set Textures
    skyboxtextures[0] = LoadTexBMP("box_front.bmp");
    skyboxtextures[1] = LoadTexBMP("box_back.bmp");
    skyboxtextures[2] = LoadTexBMP("box_left.bmp");
    skyboxtextures[3] = LoadTexBMP("box_right.bmp");
    skyboxtextures[4] = LoadTexBMP("box_top.bmp");
    skyboxtextures[5] = LoadTexBMP("box_bottom.bmp");

    // dim is distance of each face from the origin.
    // Each face has an area of (2*dim)^2
    
    // Draw Skybox
    Cube(0,0,0, 0,0, dim, skyboxtextures);
    
    // Disable textures8
    glDisable(GL_TEXTURE_2D);

    return 0;
}