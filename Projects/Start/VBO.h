#pragma once
#include <glad/glad.h>

//class for vertex buffer object
//constructor takes in:
//		vertices - pointer to the array of data that we want to feed VBO with
//		size - size of array of data that we want to feed VBO with
class VBO
{
	public:
		GLuint id;
		VBO(GLfloat* vertices, GLsizeiptr size);
		//Used for binding buffer which is associated to this object
		void Bind();
		//Used for binding buffer which is associated to this object
		void Unbind();
		//Used for deleting buffer which is associated to this object
		void Delete();
};