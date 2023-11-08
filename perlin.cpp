#include "archlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <vector>
#include <numeric>


using namespace std;

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
            nodes[i][j][0] = gradient[0];
            nodes[i][j][1] = gradient[1];
            printf("vector[%d][%d]: %f, %f \n", i, j, nodes[i][j][0], nodes[i][j][1]);
        }
    }

    // Initialize and handle point array
    vector<vector<double>> noise(pointNumber, vector<double>(pointNumber, 0));
    // Calculate the noise level of each point in the point array.
    for (int j = 0; j < pointNumber; j++) {
        for (int i = 0; i < pointNumber; i++) {
            // Find the four nodes that surround a specific point.
            float point = 0;

            int left_coord = (i/d);
            int right_coord = min((i/d) + 1, vectorNumber-1);
            int top_coord = min((j/d)+1, vectorNumber-1);
            int bottom_coord = (j/d);
            
            printf("i: %d, j: %d, left_coord: %d, right_coord: %d, top_coord: %d, bottom_coord: %d \n",i, j,  left_coord, right_coord, top_coord, bottom_coord);

            vector<double> top_left = nodes[left_coord][top_coord];
            vector<double> top_right = nodes[right_coord][top_coord];
            vector<double> bottom_left = nodes[left_coord][bottom_coord];
            vector<double> bottom_right = nodes[right_coord][bottom_coord];

            // Find the vector distance from each of the outer points.

            float c = (float)1/(float)(d+1);

            float left_wall_distance = c * ((i % d) + 1);
            float right_wall_distance = c * (d - (i % d));

            float ceiling_distance = c * (d - (j % d));
            float floor_distance = c * (j % d + 1);

            float ul_d_vec[2] = {left_wall_distance, ceiling_distance};

            float ur_d_vec[2] = {right_wall_distance, ceiling_distance};
            float bl_d_vec[2] = {left_wall_distance,floor_distance};
            float br_d_vec[2] = {right_wall_distance, floor_distance};


           

            // Find the vertical displaceent of each of the nodes
            // SKIP FOR NOW; CAN DO LATER.

            // For each point, calculate the gradients from each of the surrounding nodes. 
            // This is the dot product of the vector from the node to the point and that node's gradient.
            float tl_grad;


            printf("For point (%d, %d), the nodes are (clockwise, starting from the top right): \\
             (%d, %d), (%d, %d), (%d, %d), (%d, %d). \n", i, j, (i/d)+1, (j/d)+1, (i/d)+1, j/d, i/d, j/d, i/d, (j/d)+1);

            printf("For point (%d, %d), the distances from the nearby gradients nodes are (clockwise, starting from the top right): \\
             (%f, %f), (%f, %f), (%f, %f), (%f, %f). \n", i, j, ur_d_vec[0], ur_d_vec[1], br_d_vec[0], br_d_vec[1], bl_d_vec[0], bl_d_vec[1], ul_d_vec[0], ul_d_vec[1]);
        }
    }

    return noise;
}

int main() {
    srand(time(NULL));
    int vectorNumber = 3;
    int pointDensity = 2;
    int octaves = 1;

    vector<vector<double>> noise = Perlin2D(vectorNumber, pointDensity, octaves);

    return 0;
}