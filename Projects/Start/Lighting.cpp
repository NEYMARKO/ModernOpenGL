#include "Lighting.h"

Lighting::Lighting(Mesh& mesh, glm::vec3 lightColor) : meshContainer(mesh)
{
	this->position = mesh.objectPos;
	this->lightColor = lightColor;
	this->lightVAO = mesh.mVAO;
}

Lighting::~Lighting()
{
	std::cout << "DELETED LIGHTING" << std::endl;
}
void Lighting::Draw(Shader& shaderProgram, Camera& camera)
{
	glm::mat4 model;
	model = glm::translate(model, this->position);
	
	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);

	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("lightColor", this->lightColor);

	lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();
}