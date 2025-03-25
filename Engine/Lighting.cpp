#include "Lighting.h"

Lighting::Lighting(Mesh* mesh, const glm::vec3& position, const glm::vec3& color) 
	: mMesh {mesh}, mPosition {position}, mColor {color}
{
}

Lighting::~Lighting()
{
	std::cout << "DELETED LIGHTING" << std::endl;
}

glm::vec3 Lighting::getPosition()
{
	return mPosition;
}

glm::vec3 Lighting::getColor()
{
	return mColor;
}

void Lighting::Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera)
{
	//std::cout << "Drawing light" << std::endl;
	shaderProgram.Activate();

	glm::mat4 model = mMesh->GetFinalMatrix();
	mPosition = glm::vec3(model[3]);
	shaderProgram.SetMat4("model", model);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("lightColor", mColor);


	//there is no point in drawing same mesh again
	
	/*lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();*/

	//this->meshContainer.boundingBox->Draw(boundingBoxShaderProgram, camera);
}