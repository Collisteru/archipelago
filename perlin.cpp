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


using namespace std;

// Function Perlin for 2D noise
// Point density (also symmetrical) (pi) (Points between each vector)
// Number of octaves
// Returns a 2D array of values between the lower and upper bound with the size of the bounds for each dimensions

// Set a random vector at each point in the array
// Take the distance times the dot product of each of the vectors (?) to represent the weight to be given to each vector in the linear interolation
// Use a flattening function to flatten the number of points out. 
// Yes, I need the full array! How does the code do it without having the full array passed into the original function?

// Smoothing curve with a first and second derivative of zero at t = 0 and t = 1, making it a useful interpolation bias.
double fade(double t) {
    return (6 * pow(t, 5) - 15 * pow(t,4) + 10 * pow(t,3));
}

double joint_fade(double a, double b) {
    return fade(a) * fade(b);
}

// Linear interpolation between a and b, given a fraction t.
float lerp(float t,float a,float b) {
    return a + t * (b - a);
}

// Generate a random unit vector. This is equivalent to choosing a random point on a unit circle.
vector<double> generate_gradient() {
    // Choose theta as a random variable
    // generate the resulting coordinates with (x,y) = (cos(theta), sin(theta)), then normalize.

    rand();
    double theta = ((float)rand());

    vector<double> gradient(2);

    double x = cos(theta);
    double y = sin(theta);

    double length = sqrt((x*x)+(y*y));

    gradient[0] = x / length;
    gradient[1] = y / length;

    return gradient;
}




vector<vector<double>> Perlin2D(int vectorNumber, int pointDensity, int octaves) {

    // Initialize and handle vector array

    int pointNumber = (vectorNumber - 1) * pointDensity;

    int d = pointDensity;

    vector<vector<vector<double>>> nodes(vectorNumber, vector<vector<double>>(vectorNumber, vector<double>(2)));


    // Assign to each value in nodes a gradient
    for (int i = 0; i < vectorNumber; i++) {
        for (int j = 0; j < vectorNumber; j++) {

            vector<double> gradient = generate_gradient();

            double norm = sqrt(pow(gradient[0], 2) + pow(gradient[1], 2));

            nodes[i][j][0] = gradient[0] / norm;
            nodes[i][j][1] = gradient[1] / norm;
            
            // printf("vector[%d][%d]: %f, %f \n", i, j, nodes[i][j][0], nodes[i][j][1]);
        }
    }

    // Initialize and handle point array
    vector<vector<double>> noise(pointNumber, vector<double>(pointNumber, 0));
    // Calculate the noise level of each point in the point array.
    for (int j = 0; j < pointNumber; j++) {
        for (int i = 0; i < pointNumber; i++) {
            // Find the four nodes that surround a specific point.

            int left_coord = (i/d);
            int right_coord = min((i/d) + 1, vectorNumber-1);
            int top_coord = min((j/d)+1, vectorNumber-1);
            int bottom_coord = (j/d);
            
            // printf("i: %d, j: %d, left_coord: %d, right_coord: %d, top_coord: %d, bottom_coord: %d \n",i, j,  left_coord, right_coord, top_coord, bottom_coord);

            vector<double> top_left = {nodes[left_coord][top_coord][0], nodes[left_coord][top_coord][1]};
            vector<double> top_right = nodes[right_coord][top_coord];
            vector<double> bottom_left = nodes[left_coord][bottom_coord];
            vector<double> bottom_right = nodes[right_coord][bottom_coord];

            // Find the vector distance from each of the outer points.

            float c = (float)1/(float)(d+1);

            double left_wall_distance = c * ((i % d) + 1);
            double right_wall_distance = c * (d - (i % d));

            double ceiling_distance = c * (d - (j % d));
            double floor_distance = c * (j % d + 1);

            // printf("Surface distances: Left Wall: %f, Right Wall: %f, Ceiling: %f, Floor: %f \n", left_wall_distance, right_wall_distance, ceiling_distance, floor_distance);

            vector<double> ul_d_vec = {left_wall_distance, ceiling_distance};
            vector<double> ur_d_vec = {right_wall_distance, ceiling_distance};
            vector<double> bl_d_vec = {left_wall_distance, floor_distance};
            vector<double> br_d_vec = {right_wall_distance, floor_distance};


            // Take the dot product of the distance vectors and the gradient vectors

            // printf("ul_d_vec: (%f, %f). Top Left: (%f, %f) \n", ul_d_vec[0], ul_d_vec[1], top_left[0], top_left[1]);

            double ul_product = ul_d_vec[0] * top_left[0] + ul_d_vec[1] * top_left[1];
            double ur_product = ur_d_vec[0] * top_right[0] + ur_d_vec[1] * top_right[1];
            double bl_product = bl_d_vec[0] * bottom_left[0] + bl_d_vec[1] * bottom_left[1];
            double br_product = br_d_vec[0] * bottom_right[0] + br_d_vec[1] * bottom_right[1];

            // Find the vertical displaceent of each of the nodes
            // SKIP FOR NOW; CAN DO LATER.

            // Calculate noise via the fade function

            // printf("Dot products: Upper Left: %f, Upper Right: %f, Bottom Left: %f, Bottom Right: %f \n", ul_product, ur_product, bl_product, br_product);

            noise[i][j] = joint_fade(left_wall_distance, floor_distance) * bl_product + \
                          joint_fade(left_wall_distance, ceiling_distance) * ul_product + \
                          joint_fade(right_wall_distance, ceiling_distance) * ur_product + \
                          joint_fade(right_wall_distance, floor_distance) * br_product;


            /* printf("For point (%d, %d), the nodes are (clockwise, starting from the top right): \\
              (%d, %d), (%d, %d), (%d, %d), (%d, %d). \n", i, j, (i/d)+1, (j/d)+1, (i/d)+1, j/d, i/d, j/d, i/d, (j/d)+1); */

            /* printf("For point (%d, %d), the distances from the nearby gradients nodes are (clockwise, starting from the top right): \\
              (%f, %f), (%f, %f), (%f, %f), (%f, %f). \n", i, j, ur_d_vec[0], ur_d_vec[1], br_d_vec[0], br_d_vec[1], bl_d_vec[0], bl_d_vec[1], ul_d_vec[0], ul_d_vec[1]); */

            // printf("For point (%d, %d), the final noise value is: %f \n", i, j, noise[i][j]);
        }
    }

    return noise;
}


// Perlin Testing Suite:

// int main() {
//     srand(time(NULL));
//     int vectorNumber = 3;
//     int pointDensity = 20;
//     int octaves = 1;

//     vector<vector<double>> noise = Perlin2D(vectorNumber, pointDensity, octaves);

//     return 0;
// }