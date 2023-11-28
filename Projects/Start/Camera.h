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
		glm::vec3 cameraUp;
		glm::vec3 lookAtPosition;
		float speed;
		float sensitivity;
		float prevYPos = 0.0f;
		float prevXPos = 0.0f;
		bool focus = true;

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity);

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector);

		void calculateCameraUp();
		
		glm::mat4 LookAt(glm::vec3 targetPos);
		
		//Moves camera in direction of a 
		void Move(GLFWwindow* window, float deltaTime);

		// Rotation of camera is triggered with right mouse click
		// On release of click, rotation stops
		void Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY);
};