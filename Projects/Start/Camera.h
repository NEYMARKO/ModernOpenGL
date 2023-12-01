#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Shader.h"

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
		bool focus = false;

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity);

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector);

		void calculateCameraUp();
		
		void ViewProjectionMatrix(glm::vec3 targetPos, Shader& shaderProgram, Shader& lightShaderProgram);
		
		//Moves camera in direction of a 
		void Move(GLFWwindow* window, float deltaTime);

		//Function for camera rotation
		//Rotation starts after pressing right mouse and ends when mouse gets released
		//By hovering mouse across screen (while right mouse is pressed), camera gets rotated
		void Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY);

		void Raycast();
};