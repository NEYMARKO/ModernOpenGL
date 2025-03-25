#pragma once
#include "Mesh.h"

class Lighting
{
private:
	glm::vec3 mPosition;
	glm::vec3 mColor;
	float mSpecularStrength;
	Mesh* mMesh;
public:
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;

	//Initializes object to "mesh" and assigns it color "lightColor"
	Lighting(Mesh* mesh, const glm::vec3& position, const glm::vec3& color);
	~Lighting();

	glm::vec3 getPosition();
	glm::vec3 getColor();
	//Calculates MVP matrices, assigns uniforms and draws mesh that represents source of light
	//Uses light shader program
	void Draw(Shader& shader, Shader& boundingBoxShaderProgram, Camera& camera);
};