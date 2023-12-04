#include "Mesh.h"
#include "VAO.h"
#include "Lighting.h"
#include "VBO.h"

Lighting::Lighting(glm::vec3 position, Mesh& mesh) : meshContainer(mesh)
{
	this->position = position;
	this->lightVAO = mesh.mVAO;
}

void Lighting::Draw()
{
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