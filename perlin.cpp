#include "archlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <vector>
#include <numeric>




// Function Perlin for 2D noise=
// numVectors
// Point density (also symmetrical) (pi) (Points between each vector)
// Number of octaves
// Returns a 2D array of values between the lower and upper bound with the size of the bounds for each dimensions

// Set a random vector at each point in the array
// Take the distance times the dot product of each of the vectors (?) to represent the weight to be given to each vector in the linear interolation
// Use a flattening function to flatten the number of points out. 
// Yes, I need the full array! How does the code do it without having the full array passed into the original function?

// Smoothing curve with a derivative of zero at t = 0 and t = 1, making it a useful interpolation bias.
float smoothstep(int t) {
    return t * t * (3. - 2. * t);
}

// Linear interpolation between a and b, given a fraction t.
float lerp(float t,float a,float b) {
    return a + t * (b - a);
}

float fading_function(float t) {
    return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

// Generate a random unit vector. This is equivalent to choosing a random point on a unit circle.
float* generate_gradient(float gradient[2]) {
    // Choose theta as a random variable
    // generate the resulting coordinates with (x,y) = (cos(theta), sin(theta)), then normalize.

    rand();
    float theta = ((float)rand());

    double x = cos(theta);
    double y = sin(theta);

    double length = sqrt((x*x)+(y*y));

    gradient[0] = x / length;
    gradient[1] = y / length;

    return gradient;
}


float** Perlin2D(int vectorNumber, int pointDensity, int octaves) {

    // Initialize and handle vector array

    int pointNumber = vectorNumber * pointDensity;

    int d = pointDensity;

    float*** nodes = (float***)malloc(vectorNumber * sizeof(float*));

    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < vectorNumber; i++) {
        nodes[i] = (float**)malloc(vectorNumber * sizeof(float*));
        if (nodes[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            // You may need to deallocate previously allocated memory here.
            return NULL;
        }

        for (int j = 0; j < vectorNumber; j++) {
            nodes[i][j] = (float*)malloc(2 * sizeof(float));
            if (nodes[i][j] == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                // Deallocate previously allocated memory here.
                return NULL;
            }
        }
    }

    // Assign to each value in nodes a gradient
    for (int i = 0; i < vectorNumber; i++) {
        for (int j = 0; j < vectorNumber; j++) {
            nodes[i][j] = generate_gradient(nodes[i][j]);
            printf("vector[%d][%d]: %f, %f \n", i, j, nodes[i][j][0], nodes[i][j][1]);
        }
    }

    // Initialize and handle point array
    float** noise = (float**)malloc(pointNumber * sizeof(float*));

    if (noise == NULL) {
        fprintf(stderr, "Point Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < pointNumber; i++) {
        noise[i] = (float*)malloc(pointNumber * sizeof(float));
        if (noise[i] == NULL) {
            fprintf(stderr, "Point Memory allocation failed.\n");
            // You may need to deallocate previously allocated memory here.
            return NULL;
        }
    }

    // Initialize and handle elevation array
    float** elevations = (float**)malloc(pointNumber * sizeof(float*));

    if (noise == NULL) {
        fprintf(stderr, "Elevation Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < pointNumber; i++) {
        noise[i] = (float*)malloc(pointNumber * sizeof(float));
        if (noise[i] == NULL) {
            fprintf(stderr, "Elevation Memory allocation failed.\n");
            // You may need to deallocate previously allocated memory here.
            return NULL;
        }
    }

    // Calculate the noise level of each point in the point array.
    for (int j = 0; j < pointNumber; j++) {
        for (int i = 0; i < pointNumber; i++) {
            // Find the four nodes that surround a specific point.
            float point = noise[i][j];
            float* top_left = nodes[i/d][(j/d)+1];
            float* top_right = nodes[(i/d)+1][(j/d)+1];
            float* bottom_left = nodes[i/d][j/d];
            float* bottom_right = nodes[(i/d)+1][j/d];

            // Find the vector distance from each of the outer points.
            float c = (float)1/(float)(d+1);

            float ul_d_vec[2] = {c * ((i % d) + 1), c * (d - (j % d))};
            float ur_d_vec[2] = {c * (d - (i % d)), c * (d - (j % d))};
            float bl_d_vec[2] = {c * ((i % d) + 1), c * (j % d + 1)};
            float br_d_vec[2] = {c * (d - (i % d)), c * (j % d + 1)};

            // Find the vertical displaceent of each of the nodes
            // SKIP FOR NOW; CAN DO LATER.

            // For each point, calculate the gradients from each of the surrounding nodes. 
            // This is the dot product of the vector from the node to the point and that node's gradient.
            float tl_grad;


            // printf("For point (%d, %d), the nodes are (clockwise, starting from the top right): \\
            //  (%d, %d), (%d, %d), (%d, %d), (%d, %d). \n", i, j, (i/d)+1, (j/d)+1, (i/d)+1, j/d, i/d, j/d, i/d, (j/d)+1);

            // printf("For point (%d, %d), the distances from the nearby gradients nodes are (clockwise, starting from the top right): \\
            //  (%f, %f), (%f, %f), (%f, %f), (%f, %f). \n", i, j, ur_d_vec[0], ur_d_vec[1], br_d_vec[0], br_d_vec[1], bl_d_vec[0], bl_d_vec[1], ul_d_vec[0], ul_d_vec[1]);
        }
    }

    return noise;
}

int main() {
    srand(time(NULL));
    int vectorNumber = 8;
    int pointDensity = 20;
    int octaves = 1;

    float** myArray = Perlin2D(vectorNumber, pointDensity, octaves);

    // // Deallocate memory when done (causes segmentation fault)
    // for (int i = 0; i < pointNumber; i++) {
    //     free(myArray[i]);
    // }
    // free(myArray);

    return 0;
}