// Contains implementations of Flyer and its children

#include "archlib.h"
#include <vector>


Flyer::Flyer(vector<double> inputpos, vector<double> inputfor) {
    position = inputpos;
    
    // You should normalize this.forward, that is, this.forward should be the normalized version of inputfor
    forward = inputfor; // FIXME: MAKE THIS HAPPEN! 
    up[0] = 0;
    up[1] = 1;
    up[2] = 0;
}

Butterfly::Butterfly(double inputspeed, double inputturnspeed, vector<double> inputpos, vector<double> inputfor) : Flyer(inputpos, inputfor) {
    speed = inputspeed;
    turnspeed = inputturnspeed;
}

void Flyer::draw()
{
    return;
}

void Butterfly::draw(double size)
{
    // Size is the width and length of the butterfly to be made

    // Load and render texture

    // extern unsigned int butterflytextures[14];

    vector<double> orient_right = cross_product(forward, up);

    Point top_left;
    top_left.x = size * (position[0] + forward[0] - orient_right[0]);
    top_left.y =  size * (position[1] + forward[1] - orient_right[1]);
    top_left.z = size * (position[2] + forward[2] - orient_right[2]);

    Point top_right;
    top_right.x =size *  (position[0] + forward[0] + orient_right[0]);
    top_right.y = size * (position[1] + forward[1] + orient_right[1]);
    top_right.z = size * (position[2] + forward[2] + orient_right[2]);

    Point bottom_right;
    bottom_right.x = size * (position[0] - forward[0] + orient_right[0]);
    bottom_right.y = size * (position[1] - forward[1] + orient_right[1]);
    bottom_right.z = size * (position[2] - forward[2] + orient_right[2]);

    Point bottom_left;
    bottom_left.x = size * (position[0] - forward[0] - orient_right[0]);
    bottom_left.y =size *  (position[1] - forward[1] - orient_right[1]);
    bottom_left.z = size * (position[2] - forward[2] - orient_right[2]);

    

    Point T[] = { {0,1} , {1,1} , {1,0} , {0,0} };

    // butterflytextures[0] = LoadTexBMP("butter1.bmp");

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , mode?GL_REPLACE:GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, LoadTexBMP("butter6.bmp"));

    // Top Left, Top Right, Bottom Right, Bottom Left
    glColor4f(1,1,1, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(T[0].x, T[0].y); glVertex3f(top_left.x, top_left.y, top_left.z);
        glTexCoord2f(T[1].x, T[1].y); glVertex3f(top_right.x, top_right.y, top_right.z);
        glTexCoord2f(T[2].x, T[2].y); glVertex3f(bottom_right.x, bottom_right.y, bottom_right.z);
        glTexCoord2f(T[3].x, T[3].y); glVertex3f(bottom_left.x, bottom_left.y, bottom_left.z);
    glEnd();

    glDisable(GL_TEXTURE_2D);

}