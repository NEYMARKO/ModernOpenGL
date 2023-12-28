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


	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);
	
	unsigned int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
	unsigned int objectColorLocation = glGetUniformLocation(shaderProgram.ID, "objectColor");
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgram.ID, "lightColor");
	unsigned int lightPositionLocation = glGetUniformLocation(shaderProgram.ID, "lightPos");

	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = lighting.lightColor;
	glm::vec3 lightPos = lighting.position;

	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(objectColorLocation, 1, glm::value_ptr(objectColor));
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPos));

	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();
}

void Mesh::Print()
{
	std::cout << "Vertices" << std::endl;
	for (int i = 0; i < this->vertices.size(); i++)
	{
		Vertex vert = this->vertices[i];
		std::cout << "(X, Y, Z): (" << vert.position.x << ", " << vert.position.y << ", " << vert.position.z << ")" << std::endl;
	}
	std::cout << "Normals" << std::endl;
	for (int i = 0; i < this->vertices.size(); i++)
	{
		Vertex vert = this->vertices[i];
		std::cout << "(X, Y, Z): (" << vert.normal.x << ", " << vert.normal.y << ", " << vert.normal.z << ")" << std::endl;
	}
}