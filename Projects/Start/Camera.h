#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Ray.h"

class Camera
{
	private:
		float fov = 45.0f;

		glm::vec3 position;
		glm::quat rotation;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//direction doesn't matter, only thing that matters is that it is indeed x-axis (both vec3(1.0f, 0.0f, 0.0f) and vec3(-1.0f, 0.0f, 0.0f) lie in x axis) - it actually does
		//matter because it will rotate differently when we rotate 180 around y axis => up becomes down and down becomes up
		glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

	public:
		glm::vec3 lookAtPosition;
		float width;
		float height;
		float speed;
		float sensitivity;
		bool focus = false;
		Ray* ray;
		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, int width, int height);

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector, int width, int height);

		//Calculates cameraUp vector
		void CalculateCameraUp(glm::vec3 upVector);
		
		//Initializes view and projection matrices and sends them to shader as uniforms
		void ViewProjectionMatrix(Shader& shaderProgram);
		
		//Moves camera in all 4 directions and up/down
		void Move(GLFWwindow* window, float deltaTime);

		void Zoom(double amount);
		//Function for camera rotation
		//Rotation starts after pressing right mouse and ends when mouse gets released
		//By hovering mouse across screen (while right mouse is pressed), camera gets rotated
		void Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY);

		//Converts screen coordinates to world coordinates
		void ScreenToWorldCoordinates(const double mouseX, const double mouseY, glm::vec4& start, glm::vec3& direction);
		//Updates width and height parameters of camera to match width and height of viewport
		void UpdateViewportDimensions(const int& width, const int& height);

		//Turns x and y coordinate of a pixel into world position and casts a ray starting from that position
		//and going into direction of a camera
		void Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY);

		void RestartCameraParameters();
		glm::vec3 GetCameraForward();
		glm::vec3 GetCameraUp();
		glm::vec3 GetCameraRight();
		glm::quat GetCameraRotation();

		glm::vec3 GetCameraPosition();
		~Camera();
};