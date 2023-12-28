#include "Mesh.h"
#include "VAO.h"
#include "Lighting.h"
#include "VBO.h"

Lighting::Lighting(Mesh& mesh, glm::vec3 lightColor) : meshContainer(mesh)
{
	this->position = mesh.objectPos;
	this->lightColor = lightColor;
	this->lightVAO = mesh.mVAO;
}

void Lighting::Draw(Shader& shaderProgram, Camera& camera)
{
	glm::mat4 model;
	model = glm::translate(model, this->position);
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);
	
	unsigned int lightModelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "model");
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgram.ID, "lightColor");

	glUniformMatrix4fv(lightModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(this->lightColor));

	lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();
}