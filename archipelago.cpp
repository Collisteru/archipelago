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
double mov = 0.1;

/// Up
vector<double> sursum = {0,1,0};

/// Down
vector<double> deorsum = {0,-1,0};

/// Player View Direction
vector<double> punctum = {1, 1, 1}; // The point that the player is viewing

// Forward
vector<double> pro = {-1, -1, -1};

/// Player Movement Vectors
vector<double> sinister(3); // Left
vector<double> dexter(3); // Right
vector<double> retrorsum(3); // Backwards

// Light Settings
int    zh=0;      // Light azimuth
float  Ylight=2;  // Elevation of light
int    light = 0;     // Light mode: true=draw polygon, false=draw shadow volume


// Display Settings
int mode = 0;
double dim=3; // Size of World
Point Lp;
Point Nc,Ec;
int    moveFlag=1;    // Light movement
int    axes=1;    // Display axes
float Lpos[4];

// Perlin Noise Terrain Variables
int vectorNumber = 10;
int pointDensity = 25;
int octaves = 4;
double scale = 1.4; // The scale by which to amplify terrain height
vector<vector<double>> noise;

// Other
int    inf=1;     // Infinity
double asp=1;     // Aspect ratio

#define MAXN 64    // Maximum number of slices (n) and points in a polygon

using namespace std;

//function to calculate cross product of two vectors
vector<double> cross_product(vector<double> vector_a, vector<double> vector_b) {
   vector<double> output = {0,0,0};
   output[0] = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
   output[1] = -(vector_a[0] * vector_b[2] - vector_a[2] * vector_b[0]);
   output[2] = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];
   return output;
}

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
      glColor3f(0.023f,0.258f,0.449f);
      double ylevel = 0.1;
      glVertex3f(xmin,ylevel, zmin);
      glVertex3f(xmax,ylevel,zmin);
      glVertex3f(xmax,ylevel,zmax);
      glVertex3f(xmin,ylevel,zmax);
   glEnd();

   // Draw Fliers

   vector<double> eaglepos = {1, 3, 1};
   vector<double> eaglefor = {1, 0, 0};
   Flyer eagle(eaglepos, eaglefor);
   eagle.draw();

   //  Disable textures
   if (light) glDisable(GL_TEXTURE_2D);

   //  The floor, ceiling and walls don't cast a shadow, so bail here
   if (!light) return;

   //  Always enable textures
   glEnable(GL_TEXTURE_2D);
   Color(1,1,1);

   // Create Textures here

   //  Disable textures
   glDisable(GL_TEXTURE_2D);
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
   Scene(1);

   //  Make color buffer and Z buffer read-only
   glColorMask(0,0,0,0);
   glDepthMask(0);
   //  Enable stencil
   glEnable(GL_STENCIL_TEST);
   //  Always draw regardless of the stencil value
   glStencilFunc(GL_ALWAYS,0,0xFFFFFFFF);

   //  Enable face culling
   glEnable(GL_CULL_FACE);

   //  Disable face culling
   glDisable(GL_CULL_FACE);
   //  Make color mask and depth buffer read-write
   glColorMask(1,1,1,1);
   //  Update the color only where the stencil value is 0
   //  Do not change the stencil
   glStencilFunc(GL_EQUAL,0,0xFFFFFFFF);
   glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
   //  Redraw only if depth matches front object
   glDepthFunc(GL_EQUAL);

   //  Undo changes (no stencil test, draw if closer and sursumdate Z-buffer)
   glDisable(GL_STENCIL_TEST);
   glDepthFunc(GL_LESS); 
   glDepthMask(1);
   //  Disable lighting
   Light(0);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   // Direction of Line of Sight (Forward)
   // Conversion of spherical to rectangular coordinates where phi is the azimuth and theta is the altitude
   pro[0] = -0.01 + cos(th) * cos(ph);
   pro[1] = -0.01  + sin(th);
   pro[2] = -0.01 + cos(th) * sin(ph);

   // Punctum is direction + position
   punctum[0] = pos[0] + pro[0];
   punctum[1] = pos[1] + pro[1];
   punctum[2] = pos[2] + pro[2];

   // retrorsum = -pro
   retrorsum[0] = -pro[0];
   retrorsum[1] = -pro[1];
   retrorsum[2] = -pro[2];

   // // Direction of Right (Cross Product of Forward and Up)
   dexter = cross_product(pro, sursum);

   // Left = -dexter
   sinister[0] = -dexter[0];
   sinister[1] = -dexter[1];
   sinister[2] = -dexter[2];

   //  Erase the window and the depth and stencil buffers
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Set perspective
   glLoadIdentity();
   gluLookAt(pos[0],pos[1],pos[2] , punctum[0],punctum[1], punctum[2] , 0,1,0);


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
      ph += 0.04;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      ph -= 0.04;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      th += 0.04;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      th -= 0.04;
   //  Update projection
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
      pos[0] = pos[0] + mov * pro[0];
      pos[1] = pos[1] + mov * pro[1];
      pos[2] = pos[2] + mov * pro[2];
   }
   // Move backward
   else if (ch == 's') {
      pos[0] = pos[0] + mov * retrorsum[0];
      pos[1] = pos[1] + mov * retrorsum[1];
      pos[2] = pos[2] + mov * retrorsum[2];
   }
   // Move left
   else if (ch == 'a') {
      pos[0] = pos[0] + mov * sinister[0];
      pos[1] = pos[1] + mov * sinister[1];
      pos[2] = pos[2] + mov * sinister[2];
   }
      // Move right
   else if (ch == 'd') {
      pos[0] = pos[0] + mov * dexter[0];
      pos[1] = pos[1] + mov * dexter[1];
      pos[2] = pos[2] + mov * dexter[2];
   }
   // Move up
   else if (ch == 'u') {
      pos[0] = pos[0] + mov * sursum[0];
      pos[1] = pos[1] + mov * sursum[1];
      pos[2] = pos[2] + mov * sursum[2];
   }
   // Move down
   else if (ch == 'j') {
      pos[0] = pos[0] + mov * deorsum[0];
      pos[1] = pos[1] + mov * deorsum[1];
      pos[2] = pos[2] + mov * deorsum[2];
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
   //  Reproject
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
   //  Set projection
   Project(60,asp,dim);
}

/*
 *  Start sursum GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);

   // printf("Beginning noise generation...");

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

   glutMainLoop();
   return 0;
}
