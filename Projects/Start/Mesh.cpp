#include "Mesh.h"
#include "VBO.h"
#include "EBO.h"
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
	std::cout << "DELETED ALL BUFFERS" << std::endl;
}

void Mesh::setupMesh()
{
	//VAO VAO1;
	meshVAO.Bind();

	VBO VBO1(vertices);
	EBO EBO1(indices);

	//this->_VAO = VAO1.ID;
	this->_VAO = meshVAO.ID;
	this->_VBO = VBO1.ID;
	this->_EBO = EBO1.ID;

	meshVAO.LinkVBO(VBO1, 0, sizeof(Vertex), 0);
	meshVAO.LinkVBO(VBO1, 1, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

	//VAO1.Unbind();
	meshVAO.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

}

void Mesh::Draw(Shader& shader)
{
	std::cout << "Something";
}