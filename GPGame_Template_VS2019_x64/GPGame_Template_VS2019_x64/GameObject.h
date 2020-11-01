#pragma once
#ifndef BROOH

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <iostream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"
#include "shapes.h"
#include "particle.h"

#endif // !BROOH
// TYPES
	/* Cube is 1;
	   Sphere is 2;
	   Cylinder is 3;
	   Arrow is 4;
	   Line is 5;
	*/

	// SUBTYPES - Define the specific shape for the figure
		/*
		Cube :
		1  to transform them into predefined walls;
		2 to make a ground;
		Sphere:
		1 to make them as little as particles;
		Cylinder
		1 to reduce them to the size and length of predefined sticks.
		*/

class GameObject
{
public:
	glm::vec3	worldcenter_position;
	glm::vec3	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3	scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3	rotation = glm::vec3(0.0f, .0f, 1.0f);
	glm::vec3	rotation_2 = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3	rotation_3 = glm::vec3(0.0f, 0.0f, 0.0f);

	//Important vectors for collision testing
	glm::vec3	collision_scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3	max_figure_values = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3	min_figure_values = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3	possible_translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3	possible_scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3	possible_rotation = glm::vec3(0.0f, .0f, 1.0f);
	float		possible_angle = 0.0f;



	int				id;
	int				type;
	int				subtype;
	float			angle = 0.0f;

	glm::vec4		line_color;
	glm::vec4		fill_color;

	Shapes			figure;
	Shapes			collision;
	vector<Particle*> particles;
	vector<Shapes> particles_collisions;
	char			normal_collision;
	int nbPcl;

	glm::vec3 pos0;
	glm::vec3 position;

	int  lifespan;
	bool dead;
	bool touched = false;
	glm::vec3	touched_plan;

	//Methods
	GameObject(int id, int type, int subtype);
	void CollisionBox();
	void figure_center(int mode);
	void update_collision_scale_and_values();
	glm::vec4 	matrices_mul(glm::mat4, glm::vec4);
	void createFountain(Shapes shape, int nb);
	void createBalls(Shapes shape, int nb);
	void update_possible_transformation(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling, float a_angle);

};