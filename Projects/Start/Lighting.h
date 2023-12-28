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
	float specularStrength;
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;
	Mesh meshContainer;

	//Initializes object to "mesh" and assigns it color "lightColor"
	Lighting(Mesh& mesh, glm::vec3 lightColor);
	//Calculates MVP matrices, assigns uniforms and draws mesh that represents source of light
	//Uses light shader program
	void Draw(Shader& shader, Camera& camera);
};