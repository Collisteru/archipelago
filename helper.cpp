// File for rendering the skybox in the scene

#include "archlib.h"
#include <string>

// Get elapsed time since program start
double getTime() {
   //  Elapsed time in milliseconds
   double t = glutGet(GLUT_ELAPSED_TIME);
   return t;
}

//function to calculate cross forwardsduct of two vectors
vector<double> cross_product(vector<double> vector_a, vector<double> vector_b) {
   vector<double> output = {0,0,0};
   output[0] = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
   output[1] = -(vector_a[0] * vector_b[2] - vector_a[2] * vector_b[0]);
   output[2] = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];
   return output;
}