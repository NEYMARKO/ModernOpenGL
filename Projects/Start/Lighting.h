#pragma once
#include <iostream>
#include <string>
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VAO;
class Mesh;

class Lighting
{
public:
	glm::vec3 position;
	glm::vec3 lightColor;
	float ambientStrength;
	float specularStrength;
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;
	Mesh meshContainer;

	//Assigns position and mesh to the lighting
	//If mesh is not defined, cube is used as default
	Lighting(glm::vec3 position, Mesh& mesh);
	//Uses information about lighting to position it and draw object
	//that represents lighting source.
	void Draw();
	glm::mat4 ModelMatrix();
};