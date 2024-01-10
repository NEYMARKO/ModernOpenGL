#include "Camera.h"
#include "Ray.h"

Ray::Ray(glm::vec3 rayStart, glm::vec3 rayDirection, float rayLength)
{
	this->start = rayStart;
	this->direction = rayDirection;
	this->length = rayLength;

	this->vertices = {
		this->start,
		this->direction * this->length
	};

	this->indices = { 0, 1 };
	SetupBuffers();
	//std::cout << "Ray created" << std::endl;
}

Ray::~Ray()
{
	rayVAO.Delete();
	rayVBO.Delete();
	rayEBO.Delete();

	//std::cout << "Ray deleted" << std::endl;
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

void Ray::UpdatePosition(glm::vec3 start)
{
	this->start = start;
}

void Ray::UpdateDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void Ray::Draw(Shader& shaderProgram, Camera& camera)
{
	//std::cout << "Drawing ray" << std::endl;
	shaderProgram.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->start);

	shaderProgram.SetMat4("model", model);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));

	rayVAO.Bind();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	rayVAO.Unbind();
}