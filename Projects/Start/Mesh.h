#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

//[pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, textx, texy]


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class Mesh
{
	private:
		VBO _VBO;
		EBO _EBO;
		void setupMesh();
	public:
		VAO _VAO;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		//Deconstructor for Mesh object
		//Deletes VAO, VBO and EBO associated with mesh
		~Mesh();
		//Binds EBO and draws Mesh
		void Draw(Shader& shader);
		//Returns VBO ID
		unsigned int getVBO();
		//Returns EBO ID
		unsigned int getEBO();
};