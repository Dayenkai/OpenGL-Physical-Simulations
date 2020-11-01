#pragma once
#include "particle.h"
class Emitter
{
public:
	Particle p[360];

	void create(int nb);

	int nbPcl;
};

/*
float orientation; // how many degrees are added from the x asis on the (x,z) plan
	float birthTime;

	void initPcl(glm::vec3 vec, float o);
	void updatePcl();
	void setColor();

	figure = Sphere();
		scaling = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
		if (figure_type_param == 1)
			figure = Cube();
		if (figure_type_param == 2)
			figure = Cylinder();
		break;
*/