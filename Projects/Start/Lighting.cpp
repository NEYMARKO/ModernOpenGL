#include "Mesh.h"
#include "VAO.h"
#include "Lighting.h"
#include "VBO.h"

Lighting::Lighting(glm::vec3 position, Mesh& mesh) : meshContainer(mesh)
{
	this->position = position;
	this->lightVAO = mesh.mVAO;
}

void Lighting::Draw(Shader& shaderProgram, Camera& camera)
{
	//meshContainer.Draw(shaderProgram, camera, this->position);
	glm::mat4 lightModelMatrix = ModelMatrix();
	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);
	unsigned int lightModelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(lightModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightModelMatrix));

	lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();
}
glm::mat4 Lighting::ModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, this->position);
	return model;
}