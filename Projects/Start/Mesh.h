#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "MeshLoader.h"
#include "BoundingBox.h"

class Lighting;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class Mesh
{
	private:
		VBO mVBO;
		EBO mEBO;
		//Binds VAO and fills VBO and EBO with data from vertices and indices vectors
		//Links VBO attributes 
		//Unbinds VAO, VBO, EBO
		void setupMesh();
		float id;
	public:
		VAO mVAO;
		BoundingBox* boundingBox;
		glm::vec3 objectPos;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		float scalingFactor;
		//Constructor for Mesh - initializes vertices and indices vectors
		Mesh(const char* filePath, glm::vec3 objectPos, float id);
		//Deconstructor for Mesh object
		//Deletes VAO, VBO and EBO associated with mesh
		~Mesh();
		//Binds VAO, calculates MVP matrices, assigns uniforms and draws object using EBO info
		void Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting);
};