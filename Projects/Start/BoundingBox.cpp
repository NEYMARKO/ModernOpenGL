#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh) : parentMesh(parentMesh), boxVAO(parentMesh.mVAO)
{
	//Bounds: [xmin, xmax, ymin, ymax, zmin, zmax]
	for (int i = 0; i < 3; i++)
	{
		this->bounds.push_back(minExtremes[i]);
		this->bounds.push_back(maxExtremes[i]);
	}
	Initialize();
	SetupBuffers();
}

void BoundingBox::Initialize()
{
	float xmin = this->bounds[0];
	float ymin = this->bounds[1];
	float zmin = this->bounds[2];
	float xmax = this->bounds[3];
	float ymax = this->bounds[4];
	float zmax = this->bounds[5];

	this->vertices = {
		glm::vec3(xmin, ymin, zmin), // 0
		glm::vec3(xmax, ymin, zmin), // 1
		glm::vec3(xmax, ymax, zmin), // 2
		glm::vec3(xmin, ymax, zmin), // 3
		glm::vec3(xmin, ymin, zmax), // 4
		glm::vec3(xmax, ymin, zmax), // 5
		glm::vec3(xmax, ymax, zmax), // 6
		glm::vec3(xmin, ymax, zmax)  // 7
	};

	this->indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
}

void BoundingBox::SetupBuffers()
{
	this->boxVAO.Bind();
	this->boxVBO = VBO(this->vertices);
	this->boxEBO = EBO(this->indices);

	this->boxVAO.LinkVBO(boxVBO, 0, sizeof(glm::vec3), 0);

	this->boxVAO.Unbind();
	this->boxVBO.Unbind();
	this->boxEBO.Unbind();
}

void BoundingBox::Draw(Shader& shaderProgram, Camera& camera)
{

}