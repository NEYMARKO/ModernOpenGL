#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// Camera object class
//	- constructor takes in camera position, target position and world up vector (optional - if not defined, set to (0.0f, 1.0f, 0.0f))
class Camera
{
	public:
		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 targetPos;
		glm::vec3 cameraPos;
		glm::vec3 cameraDirection;
		float speed;
		float sensitivity;


		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity);

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector);

		glm::mat4 LookAt(glm::vec3 targetPos);
		
		void Move(GLFWwindow* window);

		void Rotate();
};