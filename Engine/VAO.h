#pragma once
#include "OpenGLIncludes.h"

class VBO;

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	// Binds VBO to VAO and creates VBO attribute at (location = layout)
	//Size - number of components per generic vertex attribute (for example: position has 3 (xyz), color has 4 (rgba))
	void LinkVBO(VBO& VBO, GLuint layout, GLint size, GLsizei stride, const GLvoid* pointer);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};