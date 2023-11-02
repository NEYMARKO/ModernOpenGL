#pragma once
#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	// Binds VBO to VAO and creates VBO attribute at (location = layout)
	void LinkVBO(VBO& VBO, GLuint layout, char dataType);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};