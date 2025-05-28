#pragma once
#include <vector>
#include "OpenGLIncludes.h"
#include <glm/glm.hpp>
struct Vertex;
//class for vertex buffer object
//constructor takes in:
//		vertices - pointer to the array of data that we want to feed VBO with
//		size - size of array of data that we want to feed VBO with
class VBO
{
	public:
		size_t m_storageCapacity;
		GLuint ID;
		//This needs to exist because of declaration of VBO in private spectar of Mesh and BoundingBox
		VBO();
		VBO(size_t capacity);
		VBO(const std::vector<glm::vec3>& verticesPositions);
		VBO(const std::vector<Vertex>& vertices);
		//~VBO();
		//returns size of storage occupied by VBO in bytes - needs to be divided
		//with type of elements that it is holding (for example: Vertex or glm::vec3)
		//to determine number of elements that are being stored. Example of usage:
		//int numberOfElements = vbo.getSizeInBytes() / sizeof(Vertex) if vbo is storing Vertex
		//objects
		int getSizeInBytes();
		//Used for binding buffer which is associated to this object
		void Bind();
		//Used for binding buffer which is associated to this object
		void Unbind();
		//Used for deleting buffer which is associated to this object
		void Delete();
};