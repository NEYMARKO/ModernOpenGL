#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
}

Mesh::~Mesh()
{
	_VAO.Delete();
	_VBO.Delete();
	_EBO.Delete();
	/*glDeleteBuffers(1, &_VAO.ID);
	glDeleteBuffers(1, &_VBO.ID);
	glDeleteBuffers(1, &_EBO.ID);*/
	std::cout << "DELETED ALL BUFFERS" << std::endl;
}

void Mesh::setupMesh()
{
	//VAO VAO1;
	this->_VAO.Bind();
	this->_VBO = VBO(vertices);
	this->_EBO = EBO(indices);

	this->_VAO.LinkVBO(_VBO, 0, sizeof(Vertex), 0);

	this->_VAO.Unbind();
	this->_VBO.Unbind();
	this->_EBO.Unbind();

}

void Mesh::Draw(Shader& shader)
{
	_VAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	_VAO.Unbind();
}

unsigned int Mesh::getVBO()
{
	return this->_VBO.ID;
}

unsigned int Mesh::getEBO()
{
	return this->_EBO.ID;
}