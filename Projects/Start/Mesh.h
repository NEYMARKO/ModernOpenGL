#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VAO.h"

//[pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, textx, texy]
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class Mesh
{
	private:
		//since VAO, VBO and EBO headers are included, VAO is reffering to
		//object, so variables shouldn't be named same as objects
		unsigned int _VAO, _VBO, _EBO;
		void setupMesh();
	public:
		VAO meshVAO;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		~Mesh();
		void Draw(Shader& shader);
		unsigned int getVBO();
		unsigned int getEBO();
};