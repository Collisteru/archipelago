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

// Local Variables
int    obj=15;    // Display objects (bitmap)
int    move=1;    // Light movement
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

// Global Variables (declared in archlib.c)
int mode = 0;
int dim=3; // Size of World
Point Lp;
Point Nc,Ec;
float Lpos[4];

//  Mode text
const char* text[] = {"Shadowed Object","Front Shadows","Back Shadows","Lit Object","Z-pass","Z-fail"};

#define MAXN 64    // Maximum number of slices (n) and points in a polygon



/*
 *  Evaluate 2D Bezier surface
 */
#define Bezier(x)  V*V*V*(U*U*U*p[0][0].x + 3*U*U*u*p[0][1].x + 3*U*u*u*p[0][2].x + u*u*u*p[0][3].x) \
               + 3*V*V*v*(U*U*U*p[1][0].x + 3*U*U*u*p[1][1].x + 3*U*u*u*p[1][2].x + u*u*u*p[1][3].x) \
               + 3*V*v*v*(U*U*U*p[2][0].x + 3*U*U*u*p[2][1].x + 3*U*u*u*p[2][2].x + u*u*u*p[2][3].x) \
               +   v*v*v*(U*U*U*p[3][0].x + 3*U*U*u*p[3][1].x + 3*U*u*u*p[3][2].x + u*u*u*p[3][3].x)
Point Bezier2D(Point p[4][4],float u,float v)
{
   float U = 1-u;
   float V = 1-v;
   Point P;
   P.x = Bezier(x);
   P.y = Bezier(y);
   P.z = Bezier(z);
   return P;
}

/*
 *  Evaluate 2D Bezier normal
 */
#define ddu(x)  -U*U*(V*V*V*p[0][0].x + 3*V*V*v*p[1][0].x + 3*V*v*v*p[2][0].x + v*v*v*p[3][0].x) \
         + (1-3*u)*U*(V*V*V*p[0][1].x + 3*V*V*v*p[1][1].x + 3*V*v*v*p[2][1].x + v*v*v*p[3][1].x) \
         + u*(2-3*u)*(V*V*V*p[0][2].x + 3*V*V*v*p[1][2].x + 3*V*v*v*p[2][2].x + v*v*v*p[3][2].x) \
         +       u*u*(V*V*V*p[0][3].x + 3*V*V*v*p[1][3].x + 3*V*v*v*p[2][3].x + v*v*v*p[3][3].x)
#define ddv(x)  -V*V*(U*U*U*p[0][0].x + 3*U*U*u*p[0][1].x + 3*U*u*u*p[0][2].x + u*u*u*p[0][3].x) \
         + (1-3*v)*V*(U*U*U*p[1][0].x + 3*U*U*u*p[1][1].x + 3*U*u*u*p[1][2].x + u*u*u*p[1][3].x) \
         + v*(2-3*v)*(U*U*U*p[2][0].x + 3*U*U*u*p[2][1].x + 3*U*u*u*p[2][2].x + u*u*u*p[2][3].x) \
         +       v*v*(U*U*U*p[3][0].x + 3*U*U*u*p[3][1].x + 3*U*u*u*p[3][2].x + u*u*u*p[3][3].x)
Point Normal2D(Point p[4][4],float u,float v)
{
   float tiny=1e-6;
   float U = 1-u;
   float V = 1-v;
   float D,Du,Dv;
   Point P,Pu,Pv;
   //  1/3 of derivative in the u direction
   Pu.x = ddu(x);
   Pu.y = ddu(y);
   Pu.z = ddu(z);
   Du = sqrt(Pu.x*Pu.x+Pu.y*Pu.y+Pu.z*Pu.z);
   //  1/3 of derivative in the v direction
   Pv.x = ddv(x);
   Pv.y = ddv(y);
   Pv.z = ddv(z);
   Dv = sqrt(Pv.x*Pv.x+Pv.y*Pv.y+Pv.z*Pv.z);
   //  Du=0
   if (Du<tiny && Dv>tiny)
   {
      u += 0.001;
      U -= 0.001;
      Pu.x = ddv(x);
      Pu.y = ddv(y);
      Pu.z = ddv(z);
   }
   //  Dv=0
   else if (Dv<tiny && Du>tiny)
   {
      v += 0.001;
      V -= 0.001;
      Pv.x = ddu(x);
      Pv.y = ddu(y);
      Pv.z = ddu(z);
   }
   //  Cross product
   P.x = Pu.y*Pv.z - Pu.z*Pv.y;
   P.y = Pu.z*Pv.x - Pu.x*Pv.z;
   P.z = Pu.x*Pv.y - Pu.y*Pv.x;
   //  Normalize
   D = sqrt(P.x*P.x+P.y*P.y+P.z*P.z);
   if (D>tiny)
   {
      P.x /= D;
      P.y /= D;
      P.z /= D;
   }
   return P;
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
      float Emission[] = {1.0,1.0,1.0,1.0};

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
   int k;  // Counters used to draw floor
 
   //  Set global light switch (used in DrawPolyShadow)
   light = Light;

   //  Texture (pi.bmp)
   glBindTexture(GL_TEXTURE_2D,tex2d[2]);
   //  Enable textures if not drawing shadow volumes
   if (light) glEnable(GL_TEXTURE_2D);

   //  Draw objects         x    y   z          th,ph    dims   
   if (obj&0x01)     Cube(-0.8,+0.8,0.0 , -0.25*zh, 0  , 0.3    );
   if (obj&0x02) Cylinder(+0.8,+0.5,0.0 ,   0.5*zh,zh  , 0.2,0.5);
   if (obj&0x04)    Torus(+0.5,-0.8,0.0 ,        0,zh  , 0.5,0.2);

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
   Light(1);
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
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(Lpos[0],Lpos[1],Lpos[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();

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
   // glWindowPos2i(5,5);
   // Print("Ylight=%.1f Angle=%d,%d,%d  Dim=%.1f Slices=%d Mode=%s Inf=%s Stencil=%d",
   //   Ylight,th,ph,zh,dim,n,text[mode],inf?"Fixed":"Calculated",depth);

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
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
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
      move = 1-move;
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
   //  Set idle function
   glutIdleFunc(move?idle:NULL);
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
   // Initialize Global Variables:
   // Globals taken from header file

   //  Initialize GLUT
   glutInit(&argc,argv);

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
   glutIdleFunc(move?idle:NULL);

   //  Check stencil depth
   glGetIntegerv(GL_STENCIL_BITS,&depth);
   if (depth<=0) Fatal("No stencil buffer\n");

   //  Load textures
   tex2d[0] = LoadTexBMP("water.bmp");
   tex2d[1] = LoadTexBMP("crate.bmp");
   tex2d[2] = LoadTexBMP("pi.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}