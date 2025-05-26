#include "VBO.h"
#include "Vertex.h"
VBO::VBO()
{

}

VBO::VBO(size_t capacity) : m_maxCapacity { capacity }
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_STATIC_DRAW);
}

VBO::VBO(const std::vector<glm::vec3>& verticesPositions)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, verticesPositions.size() * sizeof(glm::vec3), &verticesPositions[0], GL_STATIC_DRAW);
}

VBO::VBO(const std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

int VBO::getSizeInBytes()
{
	int size;
	Bind();
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	Unbind();
	return size;
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}