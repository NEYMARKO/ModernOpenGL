#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// Camera object class
//	- constructor takes in camera position, target position and world up vector (optional - if not defined, set to (0.0f, 1.0f, 0.0f))
class Camera
{
	public:
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

		//Calculates cameraUp vector
		void calculateCameraUp(glm::vec3 upVector);
		
		//Initializes view and projection matrices and sends them to shader as uniforms
		void ViewProjectionMatrix(glm::vec3 targetPos, Shader& shaderProgram);
		
		//Moves camera in all 4 directions and up/down
		void Move(GLFWwindow* window, float deltaTime);

		//Function for camera rotation
		//Rotation starts after pressing right mouse and ends when mouse gets released
		//By hovering mouse across screen (while right mouse is pressed), camera gets rotated
		void Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY);

		void Raycast();
};