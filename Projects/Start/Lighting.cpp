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
void Lighting::Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera)
{
	//std::cout << "Drawing light" << std::endl;
	shaderProgram.Activate();

	glm::mat4 model = this->meshContainer.GetFinalMatrix();
	this->position = glm::vec3(model[3]);
	shaderProgram.SetMat4("model", model);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("lightColor", this->lightColor);

	lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();

	//this->meshContainer.boundingBox->Draw(boundingBoxShaderProgram, camera);
}