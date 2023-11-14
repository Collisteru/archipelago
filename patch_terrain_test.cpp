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
#include <windows.h>

#include <iostream>
#include <random>
#include <chrono>


// Local Variables
int    obj=15;    // Display objects (bitmap)
int    moveFlag=1;    // Light movement
int    axes=1;    // Display axes
int    box=0;     // Display enclosing box
int    th=-30;    // Azimuth of view angle
int    ph=+30;    // Elevation of view angle
int    inf=1;     // Infinity
int    tex2d[3];  // Textures (names)
int    size=0;    // Lid stretch
int    depth=0;   // Stencil depth
double asp=1;     // Aspect ratio

int    zh=0;      // Light azimuth
float  Ylight=2;  // Elevation of light
int    light = 0;     // Light mode: true=draw polygon, false=draw shadow volume

int numPoints = 100; // Number of points for the vector patch

vector<vector<vector<double>>> points(numPoints, vector<vector<double>>(numPoints, vector<double>(3)));


// Global Variables (declared in archlib.c)
int mode = 0;
double dim=3; // Size of World
Point Lp;
Point Nc,Ec;
float Lpos[4];

// Perlin Variables
int vectorNumber = 10;
int pointDensity = 200;
vector<vector<double>> noise;

//  Mode text
const char* text[] = {"Shadowed Object","Front Shadows","Back Shadows","Lit Object","Z-pass","Z-fail"};

#define MAXN 64    // Maximum number of slices (n) and points in a polygon

using namespace std;

vector<vector<vector<double>>> genPatch(int numPoints) {

    // Use a random seed based on current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // Define the distribution for the range -0.5 to 0.5
    std::uniform_real_distribution<double> distribution(-0.5, 0.5);

    vector<double> xrange(numPoints);
    vector<double> zrange(numPoints);

    std::iota(xrange.begin(), xrange.end(), 0);

    std::iota(zrange.begin(), zrange.end(), 0);

    for (int i = 0; i < numPoints; i++) {
        for(int j = 0; j < numPoints; j++) {
            points[i][j][0] = zrange[i];
            points[i][j][1] = distribution(generator);
            points[i][j][2] = xrange[j];
        }
    }

    return points;
}

//
//  Print a patch of terrain from a point array to test terrain rendering with quads.
//
void terrainPatch(vector<vector<vector<double>>> points, int numPoints)
{
    // Generate a random number for the z value and render the vertex as one of four quad points
    glBegin(GL_QUADS);
    for (int i = 0; i < (numPoints - 1); i++) {
        for(int j = 0; j < (numPoints - 1); j++) {
            vector<double> bl = points[i][j];
            vector<double> tl = points[i][j+1];
            vector<double> tr = points[i+1][j+1];
            vector<double> br = points[i+1][j];

            glVertex3f(bl[0], bl[1], bl[2]);
            glVertex3f(tl[0], tl[1], tl[2]);
            glVertex3f(tr[0], tr[1], tr[2]);
            glVertex3f(br[0], br[1], br[2]);
        }
    }
    glEnd();
}

//
//  Print a patch of terrain from a point array to test terrain rendering with quads.
//
void basicPatch()
{

    vector<vector<double>> points(2, vector<double>(2, 0));

    glBegin(GL_QUADS);

    // z, y, x
    glVertex3f(-1,0,-1);
    glVertex3f(-1,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,0,-1);
    
    glEnd();


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
   // SEGFAULT SOMEWHERE IN SCENE
   int k;  // Counters used to draw floor
 
   //  Set global light switch (used in DrawPolyShadow)
   light = Light;

   // Generate terrain patch
   terrainPatch(points, numPoints);

   //  Texture (pi.bmp)
   glBindTexture(GL_TEXTURE_2D,tex2d[2]);
   //  Enable textures if not drawing shadow volumes
   if (light) glEnable(GL_TEXTURE_2D);

   //  Disable textures
   if (light) glDisable(GL_TEXTURE_2D);

   //  The floor, ceiling and walls don't cast a shadow, so bail here
   if (!light) return;

   //  Always enable textures
   glEnable(GL_TEXTURE_2D);
   Color(1,1,1);

   //  Water texture for floor and ceiling
   glBindTexture(GL_TEXTURE_2D,tex2d[0]);
   for (k=-1;k<=box;k+=2)
      // Wall(0,0,0, 0,90*k , 8,8,box?6:2 , 4);

   //  Crate texture for walls
   glBindTexture(GL_TEXTURE_2D,tex2d[1]);
   for (k=0;k<4*box;k++)
      // Wall(0,0,0, 90*k,0 , 8,box?6:2,8 , 1);

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

   //  Undo changes (no stencil test, draw if closer and update Z-buffer)
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
   //  Eye position
   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);

   //  Light position
   Lpos[0] = 2*Cos(zh);
   Lpos[1] = Ylight;
   Lpos[2] = 2*Sin(zh);
   Lpos[3] = 1;

   //  Erase the window and the depth and stencil buffers
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

   //  Draw light position as sphere (still no lighting here)
   // Light sphere disabled for now
   // glColor3f(1,1,1);
   // glPushMatrix();
   // glTranslated(Lpos[0],Lpos[1],Lpos[2]);
   // glutSolidSphere(0.03,10,10);
   // glPopMatrix();




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
   glEnable(GL_LIGHTING);

   glColor3f(1.0f, 1.0f, 1.0f);
   glDisable(GL_LIGHTING);
   


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
      th += 1;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 1;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 1;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
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
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle display modes
   else if (ch == 'm')
      mode = (mode+1)%6;
   else if (ch == 'M')
      mode = (mode+5)%6;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      moveFlag = 1-moveFlag;
   //  Toggle box
   else if (ch == 'b' || ch == 'B')
      box = 1-box;
   //  Toggle infinity calculation
   else if (ch == 'i' || ch == 'I')
      inf = 1-inf;
   //  Toggle objects
   else if (ch == 'o')
      obj = (obj+1)%16;
   else if (ch == 'O')
      obj = (obj+15)%16;
   //  Light elevation
   else if (ch=='-')
      Ylight -= 0.1;
   else if (ch=='+')
      Ylight += 0.1;
   //  Light azimuth
   else if (ch=='[')
      zh -= 1;
   else if (ch==']')
      zh += 1;
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
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{

   printf("Start!");
   //  Initialize GLUT
   glutInit(&argc,argv);

   printf("Generating patch!");

   points = genPatch(numPoints);


   //  Request double buffered, true color window with Z buffering & stencil at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
   glutInitWindowSize(600,600);
   glutCreateWindow("Patch Test");
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



// -------------------------------



