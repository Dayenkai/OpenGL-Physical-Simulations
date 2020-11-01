// Standard C++ libraries
#include <stdio.h> 
using namespace std;

// Helper graphic libraries
/*
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "explosion.h"
*/
#include "graphics.h"
#include "shapes.h"
#include <glm\gtx\transform.hpp>

// MAIN FUNCTIONS
void load();
void updateExplosion(Graphics myGraphics, float t);
void renderScene();

#define ParticlesNb   100
/*
// VARIABLES
bool    space;
bool    returnkey;
float   slowdown = 3.0f;
float   xspeed;
float   yspeed;
float   zoom = -35.0f;

// VAR TO EDIT
GLuint  loop;               // Misc Loop Variable
GLuint  col;                // Current Color Selection
GLuint  delay;              // Rainbow Effect Delay
GLuint  texture[1];         // Storage For Our Particle Texture
*/
// DEMO OBJECTS

typedef struct                      // Create A Structure For Particle
{
    bool    active;                 // Active (Yes/No)
    Sphere      sphere;

    float   x;                  // X Position
    float   y;                  // Y Position
    float   z;                  // Z Position

    float   xi;                 // X Direction
    float   yi;                 // Y Direction
    float   zi;                 // Z Direction

}
particles;                      // Particles Structure

particles p[ParticlesNb];

void explosion() {
}

void load() {
    int i=0;
    while (i < ParticlesNb) {
      p[i].sphere.Load();
      p[i].sphere.fillColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
      i += 1;
    }
}

void updateExplosion(Graphics myGraphics, float t) {
    int i = 0;
    while (i < ParticlesNb) {
        glm::mat4 mv_matrix_sphere =
            glm::translate(glm::vec3(2.0f, 0.5f, 0.0f)) *
            glm::mat4(1.0f);
        p[i].sphere.mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere;
        p[i].sphere.proj_matrix = myGraphics.proj_matrix;
    }
}