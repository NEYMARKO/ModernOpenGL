#pragma once
#include "../Buffers/VBO.h"
#include "../Buffers/EBO.h"
#include "../Buffers/VAO.h"
#include "glm/glm.hpp"

class Shader;
class Camera;

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