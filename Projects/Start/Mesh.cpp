#include "Mesh.h"
#include "MeshLoader.h"

Mesh::Mesh(const char* filePath)
{
	MeshLoader meshLoader(filePath, this);
	this->scalingFactor = meshLoader.scalingFactor;
	setupMesh();
}
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
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

	this->mVAO.Unbind();
	this->mVBO.Unbind();
	this->mEBO.Unbind();

}

void Mesh::Draw(Shader& shaderProgram)
{
	unsigned int objectColorLocation = glGetUniformLocation(shaderProgram.ID, "objectColor");
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgram.ID, "lightColor");
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(objectColorLocation, 1, glm::value_ptr(objectColor));
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();
}

unsigned int Mesh::getVBO()
{
	return this->mVBO.ID;
}

unsigned int Mesh::getEBO()
{
	return this->mEBO.ID;
}