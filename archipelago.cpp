/*
 *  Archipelago
 *
 * DESCRIPTION HERE!
 *
 *  Key bindings:
 *  m/M        Cycle through shadow volume steps (mode)
 *  o/O        Cycle through objects
 *  +/-        Change light elevation
 *  []         Change light position
 *  s/S        Start/stop light movement
 *  <>         Decrease/increase number of slices in objects
 *  b/B        Toggle room box
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "archlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <vector>
#include <numeric>

#include <bits/stdc++.h> 
using namespace std; 

// Player Movement
double    th= -0.323;    // Azimuth of view angle
double    ph= 0;    // Elevation of view angle

/// Player Position
vector<double> pos = {0, 2, 0};
double slowmov = 0.05;
double fastmov = 0.5;
double turnspeed = 0.12;

/// Up
vector<double> upwards = {0,1,0};

/// Down
vector<double> downwards = {0,-1,0};

/// Player View Direction
vector<double> viewdirection = {1, 1, 1}; // The point that the player is viewing

// Forward
vector<double> forwards = {-1, -1, -1};

/// Player Movement Vectors
vector<double> leftwards(3); // Left
vector<double> rightwards(3); // Right
vector<double> backwards(3); // Backwards

// Light Settings
int    zh=0;      // Light azimuth
float  Ylight=2;  // Elevation of light
int    light = 0;     // Light mode: true=draw polygon, false=draw shadow volume


// Display Settings and Declarations of extern Variables
int mode = 0;
double dim=3; // Size of World
Point Lp;
Point Nc,Ec;
int    moveFlag=1;    // Light movement
int    axes=1;    // Display axes
float Lpos[4];
unsigned int textures[6];

// Perlin Noise Terrain Variables
int vectorNumber = 17;
int pointDensity = 15;
int octaves = 4;
double scale = 1.4; // The scale by which to amplify terrain height
vector<vector<double>> noise;

// Other
int    inf=1;     // Infinity
double asp=1;     // Aspect ratio

#define MAXN 64    // Maximum number of slices (n) and points in a polygon

using namespace std;



/*
 *  Enable lighting
 */
static void Light(int on)
{
   if (on)
   {
      float Ambient[]   = {1.0,1.0,1.0,1.0};
      float Diffuse[]   = {3.0,3.0,3.0,1.0};
      // float Emission[] = {1.0,1.0,1.0,1.0};

      //  Enable lighting with normalization
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      // glLightfv(GL_LIGHT0, GL_EMISSION, Ambient); TODO: Fix this!
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_POSITION,Lpos);
      
   }
   else
      glDisable(GL_LIGHTING);
}

/*
 *  Draw scene
 *    light>0  lit colors
 *    light<0  shaded colors
 *    light=0  shadow volumes
 */
void Scene(int Light)
{
   //  Set global light switch (used in DrawPolyShadow)
   light = Light;

   // Draw Perlin Elevation
   GenerateTerrain(vectorNumber, pointDensity, noise, mode);
   
   // Draw the Ocean
   double xmin = 0 - (vectorNumber - (0.5)) / 2;
   double zmin = 0 - (vectorNumber - (0.5)) / 2;
   double xmax = -xmin;
   double zmax = -zmin;

   glBegin(GL_QUADS);
      glColor3f((58./256),(132./256),(189/256.));
      double ylevel = 0.1;
      glVertex3f(xmin,ylevel, zmin);
      glVertex3f(xmax,ylevel,zmin);
      glVertex3f(xmax,ylevel,zmax);
      glVertex3f(xmin,ylevel,zmax);
      glColor3f(1.0f,1.0f,1.0f); // Reset colors back
   glEnd();

   // Draw the Skybox
   Skybox(xmax);

   // Draw Fliers

   // First Butterfly
   vector<double> butter1pos = {1, 2, 1};
   vector<double> butter1for = {1, 1, 0};
   Butterfly butter1(1, 1, butter1pos, butter1for);
   butter1.draw(0.25);

}

/*
 *  Draw scene
 */
static void DrawScene()
{
   
   //  Enable lighting
   //  Render the parts of objects not in shadows
   //  (The mode test is for demonstrating unlit objects only)
   Light(0);

   // Draw scene with lit colors
   Scene(1);

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   // Direction of Line of Sight (Forward)
   // Conversion of spherical to rectangular coordinates where phi is the azimuth and theta is the altitude
   forwards[0] = -0.01 + cos(th) * cos(ph);
   forwards[1] = -0.01  + sin(th);
   forwards[2] = -0.01 + cos(th) * sin(ph);

   // Punctum is direction + position
   viewdirection[0] = pos[0] + forwards[0];
   viewdirection[1] = pos[1] + forwards[1];
   viewdirection[2] = pos[2] + forwards[2];

   // backwards = -forwards
   backwards[0] = -forwards[0];
   backwards[1] = -forwards[1];
   backwards[2] = -forwards[2];

   // // Direction of Right (Cross Product of Forward and Up)
   rightwards = cross_product(forwards, upwards);

   // Left = -rightwards
   leftwards[0] = -rightwards[0];
   leftwards[1] = -rightwards[1];
   leftwards[2] = -rightwards[2];

   //  Erase the window and the depth and stencil buffers
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Set perspective
   glLoadIdentity();
   gluLookAt(pos[0],pos[1],pos[2] , viewdirection[0],viewdirection[1], viewdirection[2] , 0,1,0);

   //  Draw the scene
   DrawScene();


   //  Draw axes (white)
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0; //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   glColor3f(1.0f, 1.0f, 0.0f);
   
   Print("z to zoom out. x to zoom in. \n");

   glColor3f(1.0f, 1.0f, 1.0f);


   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,1440.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      ph += turnspeed;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      ph -= turnspeed;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      th += turnspeed;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      th -= turnspeed;
   //  Update forwardsjection
   Project(60,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Move Forward
   else if (ch == 'w') {
      pos[0] = pos[0] + slowmov * forwards[0];
      pos[1] = pos[1] + slowmov * forwards[1];
      pos[2] = pos[2] + slowmov * forwards[2];
   }
   else if (ch == 'W') {
      pos[0] = pos[0] + fastmov * forwards[0];
      pos[1] = pos[1] + fastmov * forwards[1];
      pos[2] = pos[2] + fastmov * forwards[2];
   }
   // Move backward
   else if (ch == 's') {
      pos[0] = pos[0] + slowmov * backwards[0];
      pos[1] = pos[1] + slowmov * backwards[1];
      pos[2] = pos[2] + slowmov * backwards[2];
   }
   else if (ch == 'S') {
      pos[0] = pos[0] + fastmov * backwards[0];
      pos[1] = pos[1] + fastmov * backwards[1];
      pos[2] = pos[2] + fastmov * backwards[2];
   }
   // Move leftwards
   else if (ch == 'a') {
      pos[0] = pos[0] + slowmov * leftwards[0];
      pos[1] = pos[1] + slowmov * leftwards[1];
      pos[2] = pos[2] + slowmov * leftwards[2];
   }
   else if (ch == 'A') {
      pos[0] = pos[0] + fastmov * leftwards[0];
      pos[1] = pos[1] + fastmov * leftwards[1];
      pos[2] = pos[2] + fastmov * leftwards[2];
   }
      // Move rightwards
   else if (ch == 'd') {
      pos[0] = pos[0] + slowmov * rightwards[0];
      pos[1] = pos[1] + slowmov * rightwards[1];
      pos[2] = pos[2] + slowmov * rightwards[2];
   }
   else if (ch == 'D') {
      pos[0] = pos[0] + fastmov * rightwards[0];
      pos[1] = pos[1] + fastmov * rightwards[1];
      pos[2] = pos[2] + fastmov * rightwards[2];
   }
   // Move up
   else if (ch == 'u') {
      pos[0] = pos[0] + slowmov * upwards[0];
      pos[1] = pos[1] + slowmov * upwards[1];
      pos[2] = pos[2] + slowmov * upwards[2];
   }
   else if (ch == 'U') {
      pos[0] = pos[0] + fastmov * upwards[0];
      pos[1] = pos[1] + fastmov * upwards[1];
      pos[2] = pos[2] + fastmov * upwards[2];
   }
   // Move down
   else if (ch == 'j') {
      pos[0] = pos[0] + slowmov * downwards[0];
      pos[1] = pos[1] + slowmov * downwards[1];
      pos[2] = pos[2] + slowmov * downwards[2];
   }
   else if (ch == 'J') {
      pos[0] = pos[0] + fastmov * downwards[0];
      pos[1] = pos[1] + fastmov * downwards[1];
      pos[2] = pos[2] + fastmov * downwards[2];
   }
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle display modes
   else if (ch == 'm')
      mode = (mode+1)%3;
   else if (ch == 'M')
      if (mode == 0) mode = 2;
      else mode = mode - 1;
   // Zoom
   else if (ch=='z')
      dim += 0.05;
   else if (ch=='x')
      dim -= 0.05;
   //  Set idle function
   glutIdleFunc(moveFlag?idle:NULL);
   //  Reforwardsject
   Project(60,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set forwardsjection
   Project(60,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);

   // Generate Perlin Noise
   noise = Perlin2D(vectorNumber,pointDensity,octaves);

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
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(moveFlag?idle:NULL);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");

   // Enter Main Loop
   glutMainLoop();
   return 0;
}
