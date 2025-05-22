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
		GLuint ID;
		//This needs to exist because of declaration of VBO in private spectar of Mesh and BoundingBox
		VBO();
		VBO(const std::vector<glm::vec3>& verticesPositions);
		VBO(const std::vector<Vertex>& vertices);
		//Used for binding buffer which is associated to this object
		void Bind();
		//Used for binding buffer which is associated to this object
		void Unbind();
		//Used for deleting buffer which is associated to this object
		void Delete();
};