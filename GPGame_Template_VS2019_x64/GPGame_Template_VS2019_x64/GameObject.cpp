#include "GameObject.h"
#include	<math.h>
using namespace std;

GameObject::GameObject(int id_param, int object_type_param, int figure_type_param) :
	id(id_param), type(object_type_param), subtype(figure_type_param)
{
	switch (object_type_param)
	{
	case 1:
		figure = Cube();
		if (figure_type_param == 1)
			scaling = glm::vec3(1.0f, 2.0f, 0.4f);
		else if (figure_type_param == 2)
			scaling = glm::vec3(1000.0f, 0.001f, 1000.0f);
		else
			translation = glm::vec3(5.0f, 0.5f, 0.0f);
		break;
	case 2:
		figure = Sphere();
		break;
	case 3:
		figure = Cylinder();
		break;
	case 4:
		figure = Arrow();
		break;
	case 5:
		figure = Line();
		break;
	case 6:
		figure = Sphere();
		scaling = glm::vec3(0.1f, 0.1f, 0.1f);
		if (figure_type_param == 1) { //Fountain
			scaling = glm::vec3(0.1f, 0.1f, 0.1f);
			createFountain(figure, 360);
		}
		if (figure_type_param == 2) { //Bouncing balls 
			scaling = glm::vec3(0.5f, 0.5f, 0.5f);
			createBalls(figure, 1);
		}
		break;
	}
	 if (object_type_param != 6)
		 figure.Load();
	figure_center(0);
	CollisionBox();
	collision.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	collision.lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	dead = true;
	touched = false;
}

void		GameObject::CollisionBox()
{
	collision = Cube();
	collision.Load();

}


void GameObject::update_collision_scale_and_values()
{

}



glm::vec4 	GameObject::matrices_mul(glm::mat4 mv_matrix_cube, glm::vec4 coordinates)
{
	int		i = 0;
	float	vertex_position = 0;
	float		x = 0, y = 0, z = 0, w = 0;


	for (int i = 0; i < 16; i = i + 4)
	{
		if (i / 4 < 1)
		{
			vertex_position = mv_matrix_cube[0].x * coordinates.x + mv_matrix_cube[1].x * coordinates.y + mv_matrix_cube[2].x * coordinates.z + mv_matrix_cube[3].x * coordinates.w;
			x = vertex_position;
		}
		else if (i / 4 >= 1 && i / 4 < 2)
		{
			vertex_position = mv_matrix_cube[0].y * coordinates.x + mv_matrix_cube[1].y * coordinates.y + mv_matrix_cube[2].y * coordinates.z + mv_matrix_cube[3].y * coordinates.w;
			y = vertex_position;
		}
		else if (i / 4 >= 2 && i / 4 < 3)
		{
			vertex_position = mv_matrix_cube[0].z * coordinates.x + mv_matrix_cube[1].z * coordinates.y + mv_matrix_cube[2].z * coordinates.z + mv_matrix_cube[3].z * coordinates.w;
			z = vertex_position;
		}
		else if (i / 4 >= 3 && i / 4 <= 4)
		{
			vertex_position = mv_matrix_cube[0].w * coordinates.x + mv_matrix_cube[1].w * coordinates.y + mv_matrix_cube[2].w * coordinates.z + mv_matrix_cube[3].w * coordinates.w;
			w = vertex_position;
		}
		vertex_position = 0;
	}
	glm::vec4 screen_coordinates(x, y, z, w);
	return screen_coordinates;
}

void		GameObject::figure_center(int mode)
{
	int		i = 0;
	int		j = 0;
	glm::vec4	test;
	GLfloat min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	glm::mat4 mat_transformation(1.0f);

	if (mode == 0)
		mat_transformation = glm::translate(glm::vec3(translation)) * glm::rotate(glm::radians(angle), rotation) * glm::scale(scaling) * glm::mat4(1.0f);
	else if (mode == 1)
		mat_transformation = glm::translate(glm::vec3(possible_translation)) * glm::rotate(glm::radians(possible_angle), possible_rotation) * glm::scale(possible_scaling) * glm::mat4(1.0f);
	/*if (mode == 1)
		mat_transformation = glm::translate(glm::vec3(translation)) * glm::rotate(angle, rotation) * glm::scale(scaling) * glm::mat4(1.0f);*/
	/*else if (mode == 2)
		glm::mat4 mat_transformation = glm::translate(glm::vec3(translation)) * glm::rotate(angle, rotation) * glm::scale(possible_scaling) * glm::mat4(1.0f);*/

	float	x = 0, y = 0, z = 0, w = 0;
	test = matrices_mul(mat_transformation, glm::vec4(figure.vertexPositions[0], figure.vertexPositions[1], figure.vertexPositions[2], 1));
	min_x = max_x = test.x;
	min_y = max_y = test.y;
	min_z = max_z = test.z;

	vector<glm::vec4> vertices_screen_positions;
	while (i < figure.vertexPositions.size() / 3)
	{
		vertices_screen_positions.push_back(matrices_mul(mat_transformation, glm::vec4(figure.vertexPositions[j], figure.vertexPositions[j + 1], figure.vertexPositions[j + 2], 1)));
		j = j + 3;

		if (vertices_screen_positions[i].x < min_x) min_x = vertices_screen_positions[i].x;
		if (vertices_screen_positions[i].x > max_x) max_x = vertices_screen_positions[i].x;

		if (vertices_screen_positions[i].y < min_y) min_y = vertices_screen_positions[i].y;
		if (vertices_screen_positions[i].y > max_y) max_y = vertices_screen_positions[i].y;

		if (vertices_screen_positions[i].z < min_z) min_z = vertices_screen_positions[i].z;
		if (vertices_screen_positions[i].z > max_z) max_z = vertices_screen_positions[i].z;

		i++;
	}
	i = 0;
	while (i < figure.vertexPositions.size() / 3)
	{
		x = x + vertices_screen_positions[i].x;
		y = y + vertices_screen_positions[i].y;
		z = z + vertices_screen_positions[i].z;
		w = w + vertices_screen_positions[i].w;
		i++;
	}
	x = x / (figure.vertexPositions.size() / 3);
	y = y / (figure.vertexPositions.size() / 3);
	z = z / (figure.vertexPositions.size() / 3);
	w = w / (figure.vertexPositions.size() / 3);
	collision_scaling = glm::vec3(abs(max_x - min_x), abs(max_y - min_y), abs(max_z - min_z));
	glm::vec3 center_object(x, y, z);
	worldcenter_position = center_object;

	max_figure_values.x = max_x;
	max_figure_values.y = max_y;
	max_figure_values.z = max_z;
	min_figure_values.x = min_x;
	min_figure_values.y = min_y;
	min_figure_values.z = min_z;
}

void GameObject::createFountain(Shapes shape, int nb) {
	nbPcl = nb;
	float o = 0.0;
	for (int i = 0; i < nbPcl; i++) {
		particles.push_back(new Particle());
		particles[i]->id = i;
		particles[i]->init(glm::vec3(0.0f, 0.5f, 0.0f), o, shape);
		o += 1.0; // +1 degree
	}
}

void GameObject::createBalls(Shapes shape, int nb) {
	nbPcl = nb;
		particles.push_back(new Particle());
		particles[0]->id = 99;
		particles[0]->init(glm::vec3(-2.0f, 0.5f, 0.3f), 0, shape);
}


void GameObject::update_possible_transformation(glm::vec3 a_translation, glm::vec3 a_rotation, glm::vec3 a_scaling, float a_angle)
{
	possible_translation = a_translation;
	possible_rotation = a_rotation;
	possible_scaling = a_scaling;
	possible_angle = a_angle;

	figure_center(0);
}