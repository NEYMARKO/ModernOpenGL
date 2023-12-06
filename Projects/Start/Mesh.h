#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

//[pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, textx, texy]


struct Vertex
{
	glm::vec3 position;
};
//struct Vertex
//{
//	glm::vec3 position;
//	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
//};

class Mesh
{
	private:
		VBO mVBO;
		EBO mEBO;
		void setupMesh();
	public:
		VAO mVAO;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		float scalingFactor;
		//std::vector<Texture> textures;

		Mesh(const char* filePath);
		//Deconstructor for Mesh object
		//Deletes VAO, VBO and EBO associated with mesh
		~Mesh();
		//Binds VAO and draws Mesh
		void Draw(Shader& shaderProgram, Camera& camera, glm::vec3 position);
};