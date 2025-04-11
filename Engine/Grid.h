#pragma once
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"

class Grid
{
	private:
		int size;
		VBO gridVBO;
		EBO gridEBO;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		glm::mat4 modelMatrix;
	public:
		VAO gridVAO;

		Grid(int size);
		void SetupGrid();
		void Draw(Shader& shaderProgram, Camera& camera);
		~Grid();
};