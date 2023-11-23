#ifndef archlib
#define archlib

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  cos and sin in degrees
#define Cos(th) static_cast<float>(cos(3.14159265/180.0 * static_cast<float>(th)))
#define Sin(th) static_cast<float>(sin(3.14159265/180* static_cast<float>(th)))

#ifdef __cplusplus
extern "C" {
#endif




// Helper Functions
void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);


// Shapes
typedef struct {float x,y,z;} Point;
void Cube(float x,float y,float z , float th,float ph , float D);
void Cylinder(float x,float y,float z , float th,float ph , float R,float H);
void Torus(float x,float y,float z , float th,float ph , float S,float r);

// Drawing Functions
void DrawPoly(Point P[],Point N[],Point T[],int n);
void Crout(double M[16],int I[4]);
void Transform(float x0,float y0,float z0, float Sx,float Sy,float Sz, float th,float ph);
void Color(float R,float G,float B);


// Functions from perlin.c
float smoothstep(int t);
vector<double> generate_gradient();
vector<vector<double>> Perlin2D(int vectorNumber, int pointDensity, int octaves);
void TerrainTrigs(int vectorNumber, int pointDensity, vector<vector<double>> noise);
void TerrainQuads(int vectorNumber, int pointDensity, vector<vector<double>> noise);
void TerrainDots(int vectorNumber, int pointDensity, vector<vector<double>> noise);
void TerrainLines(int vectorNumber, int pointDensity, vector<vector<double>> noise);



// Global Variables Required by Files Outside archipelago.c
extern Point  Lp;        // Light position in local coordinate system
extern Point  Nc,Ec;     // Far or near clipping plane in local coordinate system

extern float  Lpos[4];   // Light position
extern int    mode;    // Display mode
extern double dim;     // Size of world



#ifdef __cplusplus
}
#endif

#endif