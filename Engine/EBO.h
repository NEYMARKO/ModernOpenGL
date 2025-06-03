#pragma once
//#include "OpenGLIncludes.h"
#include <glad/glad.h>
#include <vector>
class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	EBO();
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(const std::vector<unsigned int>& indices);
	//~EBO();
	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};
