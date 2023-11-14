
#include "archlib.h"
#include <random>
#include <chrono>


//
//  Print a patch of terrain from a point array to test terrain rendering with quads.
//
void terrainPatch(int numPoints)
{

    vector<vector<double>> points(pointNumber, vector<double>(pointNumber, 0));

    // Use a random seed based on current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // Define the distribution for the range -0.5 to 0.5
    std::uniform_real_distribution<double> distribution(-0.5, 0.5);

    // Generate a random number for the z value and render the vertex as one of four quad points
    glBegin(GL_QUADS);
    for (i = 0; i < pointNumber; i++) {
        for(j = 0; j < pointNumber; j++) {
            points[i][j] = distribution(generator);
            glVertex3f(zrange[i], noise[i][j],  xrange[j]);
        }
    }
    glEnd(GL_QUADS);
}



/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{

   //  Initialize GLUT
   glutInit(&argc,argv);
   
   terrainPatch(10);

   //  Request double buffered, true color window with Z buffering & stencil at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
   glutInitWindowSize(600,600);
   glutCreateWindow("Archipelago");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif

   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(key);
   glutIdleFunc(moveFlag?idle:NULL);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");

   glutMainLoop();
   return 0;
}

int main() {
    
    return 0;
}