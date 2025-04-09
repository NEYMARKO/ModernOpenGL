#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>

#include "Ray.h"

class Camera
{
	private:
		float mFov = 45.0f;

		glm::vec3 mPosition;
		glm::quat mRotation;

		glm::vec3 mForward;
		glm::vec3 mUp;
		glm::vec3 mRight;

		const glm::vec3 mWorldForward = glm::vec3(0.0f, 0.0f, 1.0f);
		const glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//direction doesn't matter, only thing that matters is that it is indeed x-axis (both vec3(1.0f, 0.0f, 0.0f) and vec3(-1.0f, 0.0f, 0.0f) lie in x axis) - it actually does
		//matter because it will rotate differently when we rotate 180 around y axis => up becomes down and down becomes up
		const glm::vec3 mWorldRight = glm::vec3(1.0f, 0.0f, 0.0f);


		float mXOffset{};
		float mYOffset{};
		float mSphereRadius = 5.0f;
		glm::vec3 mPointOnSphere = glm::vec3(0.0f, 0.0f, 0.0f);

	public:
		glm::vec3 mLookAtPosition;
		float mWidth;
		float mHeight;
		float mSpeed;
		float mSensitivity;
		bool focus = false;
		Ray* mRay;

		Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, 
			float sensitivity, int width, int height, 
			const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f));

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

		void calculatePointOnSphere(const double& startingX, const double& startingY, const double& currentX, const double& currentY);
		void RestartCameraParameters();

		glm::vec3 GetCameraForward() { return mForward; };
		glm::vec3 GetCameraUp() { return mUp; };
		glm::vec3 GetCameraRight() { return mRight; };
		glm::vec3 GetCameraPosition() { return mPosition; };
		glm::quat GetCameraRotation() { return mRotation; };

		~Camera();
};