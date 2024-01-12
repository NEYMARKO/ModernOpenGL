#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Shader;
class Camera;

class Ray
{	
	private:
		glm::vec3 start;
		glm::vec3 direction;
		float length;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		VBO rayVBO;
		EBO rayEBO;
	public:
		VAO rayVAO;
		Ray(glm::vec3 rayStart, glm::vec3 rayDirection, float rayLength);
		~Ray();

		void SetupBuffers();

		void UpdateData(glm::vec3& start, glm::vec3& direction);

		glm::vec3 GetRayStart();
		glm::vec3 GetRayDirection();
		float GetRayLength();

		void Draw(Shader& shaderProgram, Camera& camera);
};