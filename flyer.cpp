#include "archlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <vector>
#include <numeric>
#include <windows.h>

#include <iostream>
#include <random>
#include <chrono>

class Flyer {       // The class
  public:             
    vector<double> position = {0,0,0};
    vector<double> forward = {0,0,0};
    vector<double> up = {0,0,0};
    // Texture that this thing uses!

    // Constructor
    void Flyer(vector<double> inputpos, vector<double> inputfor) {
        position = inputpos;
        forward = inputfor;
        // Cross Product
        up[0] = position[1] * inputfor[2] - position[2] * inputfor[1];
        up[1] = -(position[0] * inputfor[2] - position[2] * inputfor[0]);
        up[2] = position[0] * inputfor[1] - position[1] * inputfor[0];
    }

    void draw()
    {
        // For now just draw a conspicuous trig
        glBegin(GL_TRIANGLES);
            glColor3f(1, 0, 0);
            glVertex3f(position[0], position[1], position[2]);
            glVertex3f(position[0]+forward[0], position[1] + forward[1], position[2] + forward[2]);
            glVertex3f(position[0]+up[0], position[1] + up[1], position[2] + up[2]);
        glEnd();
    }
};