#include "VBO.h"
#include "VAO.h"


VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout, GLint size, GLsizei stride, const GLvoid* pointer)
{
	VBO.Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}