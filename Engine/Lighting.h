#pragma once
#include "glm/glm.hpp"
#include "VAO.h"
#include "EmptyObject.h"

class Shader;
class Mesh;
class Camera;

class Lighting
{
private:
	Shader* mShaderProgram;
	Mesh* m_mesh;
	glm::vec3 m_position;
	glm::vec3 mColor;
	float mSpecularStrength;
public:
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;

	//Initializes object to "mesh" and assigns it color "lightColor"
	Lighting(Shader* shaderProgram, Mesh* mesh, const glm::vec3& position, const glm::vec3& color);
	~Lighting();

	glm::vec3 getPosition();
	glm::vec3 getColor();
	//Calculates MVP matrices, assigns uniforms and draws mesh that represents source of light
	//Uses light shader program
	void Draw(/*Shader& boundingBoxShaderProgram, */Camera& camera);
};