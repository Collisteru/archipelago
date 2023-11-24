#include "archlib.h"
#include <vector>


// Constructor
Flyer::Flyer(vector<double> inputpos, vector<double> inputfor) {
    position = inputpos;
    forward = inputfor;
    up[0] = 0;
    up[1] = 1;
    up[2] = 0;
}


void Flyer::draw()
{
    // For now just draw a conspicuous trig
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(position[0], position[1], position[2]);
        glVertex3f(position[0]+forward[0], position[1] + forward[1], position[2] + forward[2]);
        glVertex3f(position[0]+up[0], position[1] + up[1], position[2] + up[2]);
    glEnd();
}