#include "Camera.h"
#include "Ray.h"

Ray::Ray(glm::vec3 rayStart, glm::vec3 rayDirection, float rayLength)
{
	this->start = rayStart;
	this->direction = rayDirection;
	this->length = rayLength;

	this->vertices = {
		this->start,
		this->start + this->direction * this->length
	};

	this->indices = { 0, 1 };
	SetupBuffers();
}

Ray::~Ray()
{
	rayVAO.Delete();
	rayVBO.Delete();
	rayEBO.Delete();
}

void Ray::SetupBuffers()
{
	this->rayVAO.Bind();
	this->rayVBO = VBO(this->vertices);
	this->rayEBO = EBO(this->indices);

	this->rayVAO.LinkVBO(rayVBO, 0, 3, sizeof(glm::vec3), 0);

	this->rayVAO.Unbind();
	this->rayVBO.Unbind();
	this->rayEBO.Unbind();
}

void Ray::UpdateData(glm::vec3& start, glm::vec3& direction)
{
	this->start = start;
	this->direction = direction;
	
	this->vertices = {
		this->start,
		this->start + this->direction * this->length
	};
}

glm::vec3 Ray::GetRayStart()
{
	return this->start;
}

glm::vec3 Ray::GetRayDirection()
{
	return this->direction;
}

float Ray::GetRayLength()
{
	return this->length;
}

void Ray::Draw(Shader& shaderProgram, Camera& camera)
{
	shaderProgram.Activate();
	//Coordinates are already converted to world space in Raycast function
	glm::mat4 model = glm::mat4(1.0f);

	shaderProgram.SetMat4("model", model);
	camera.generateViewProjectionMatrices(shaderProgram);

	shaderProgram.SetVec3("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));

	rayVAO.Bind();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	rayVAO.Unbind();
}

VAO* Ray::GetRayVAO()
{
	return &this->rayVAO;
}

std::vector<unsigned int>* Ray::GetRayIndices()
{
	return &this->indices;
}