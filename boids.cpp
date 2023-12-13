#include "archlib.h"

#include <iostream>
#include <vector>
#include <cmath>


// Boid Constructor
Boid::Boid(double x, double y, double z) : position(x, y, z), velocity(0, 0, 0) {
    maxSpeed = 0.7;
}

// Getter methods
Vector3D Boid::getPosition() const { return position; }
Vector3D Boid::getVelocity() const { return velocity; }

// Draw this Boid
void Boid::draw() {
        glColor3f(0.502f, 0.502f, 0.502f);
        glBegin(GL_TRIANGLES);
            glVertex3f(position.x, position.y, position.z);
            glVertex3f(position.x+0.5, position.y, position.z);
            glVertex3f(position.x, position.y, position.z+0.5);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
    }

// Function to update the boid's position and velocity based on the Boids algorithm
void Boid::update(const std::vector<Boid>& boids, double separationFactor, double alignmentFactor, double cohesionFactor, double centralityFactor, double boundary) {
    Vector3D separation(0, 0, 0);
    Vector3D alignment(0, 0, 0);
    Vector3D cohesion(0, 0, 0);
    Vector3D centrality(0, 0, 0);

    int count = 0;

    for (const auto& other : boids) {
        if (&other != this) {
            double distance = sqrt(pow(position.x - other.getPosition().x, 2) +
                                    pow(position.y - other.getPosition().y, 2) +
                                    pow(position.z - other.getPosition().z, 2));

            // Separation: Move away from nearby boids
            if (distance < 4.0) {
                separation.x += (position.x - other.getPosition().x);
                separation.y += (position.y - other.getPosition().y);
                separation.z += (position.z - other.getPosition().z);
            }

            // Alignment: Align velocity with nearby boids
            if (distance < 3.0) {
                alignment.x += other.getVelocity().x;
                alignment.y += other.getVelocity().y;
                alignment.z += other.getVelocity().z;
            }

            // Cohesion: Move towards the center of nearby boids
            if (distance < 6.0) {
                cohesion.x += other.getPosition().x;
                cohesion.y += other.getPosition().y;
                cohesion.z += other.getPosition().z;
                count++;
            }
        }
    }

    
    // Centrality: Try to stay close to the center of the area
    centrality.x += position.x / boundary;
    centrality.y += position.y / boundary;
    centrality.z += position.z / boundary;


    if (count > 0) {
        cohesion.x /= count;
        cohesion.y /= count;
        cohesion.z /= count;

        cohesion.x = (cohesion.x - position.x);
        cohesion.y = (cohesion.y - position.y);
        cohesion.z = (cohesion.z - position.z);
    }

    // Apply the three rules
    separation.x *= separationFactor;
    separation.y *= separationFactor;
    separation.z *= separationFactor;

    alignment.x *= alignmentFactor;
    alignment.y *= alignmentFactor;
    alignment.z *= alignmentFactor;

    cohesion.x *= cohesionFactor;
    cohesion.y *= cohesionFactor;
    cohesion.z *= cohesionFactor;

    centrality.x *= centralityFactor;
    centrality.y *= centralityFactor;
    centrality.z *= centralityFactor;

    // Update velocity and position
    velocity.x += separation.x + alignment.x + cohesion.x - centrality.x;
    velocity.y += separation.y + alignment.y + cohesion.y - centrality.y;
    velocity.z += separation.z + alignment.z + cohesion.z - centrality.z;

    // Normalize the velocity
    double velocinorm = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

    // Keep the speed below the max speed
    if (velocinorm > maxSpeed) {
        velocity.x = velocity.x / velocinorm;
        velocity.y = velocity.y / velocinorm;
        velocity.z = velocity.z / velocinorm;

        velocity.x *= maxSpeed;
        velocity.y *= maxSpeed;
        velocity.z *= maxSpeed;
    }


    // Keep position within boundaries
    // Ensure x stays within boundaries
    if (position.x < -boundary) {
        position.x = -boundary;
        velocity.x = (-0.3) * velocity.x;
    } else if (position.x > boundary) {
        position.x = boundary;
        velocity.x = (-0.3) * velocity.x;
    }

    // Ensure y stays within boundaries
    if (position.y < 0.5) {
        position.y = 0.5;
        velocity.y = (-0.3) * velocity.y;
    } else if (position.y > boundary) {
        position.y = boundary;
        velocity.y = (-0.3) * velocity.y;
    }

    // Ensure z stays within boundaries
    if (position.z < -boundary) {
        position.z = -boundary;
        velocity.z = (-0.3) * velocity.z;
    } else if (position.z > boundary) {
        position.z = boundary;
        velocity.z = (-0.3) * velocity.z;
    }
    
    position.x += velocity.x;
    position.y += velocity.y;
    position.z += velocity.z;
}
