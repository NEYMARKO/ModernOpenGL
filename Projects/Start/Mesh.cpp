#include "Mesh.h"
#include "MeshLoader.h"
#include "Lighting.h"

Mesh::Mesh(const char* filePath, glm::vec3 objectPos)
{
	MeshLoader meshLoader(filePath, this);
	this->scalingFactor = meshLoader.scalingFactor;
	this->objectPos = objectPos;
	setupMesh();
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();
	std::cout << "DELETED ALL BUFFERS" << std::endl;
}

void Mesh::setupMesh()
{
	this->mVAO.Bind();
	this->mVBO = VBO(vertices);
	this->mEBO = EBO(indices);

	this->mVAO.LinkVBO(mVBO, 0, sizeof(Vertex), 0);
	this->mVAO.LinkVBO(mVBO, 1, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	this->mVAO.Unbind();
	this->mVBO.Unbind();
	this->mEBO.Unbind();

}

void Mesh::Draw(Shader& shaderProgram, Camera& camera, Lighting& lighting)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->objectPos);
	model = glm::scale(model, glm::vec3(this->scalingFactor, this->scalingFactor, this->scalingFactor));

	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	shaderProgram.SetVec3("lightColor", lighting.lightColor);
	shaderProgram.SetVec3("lightPost", lighting.position);

	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);
	
	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();
}
