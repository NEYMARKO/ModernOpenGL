#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>

#include "Ray.h"

/// <summary>
/// Camera is moving on the surface of the sphere. It is oriented in a way to always 
/// face the center of the sphere. Mouse position is being accumulated in offset variables 
/// when right mouse button is pressed. Those values are used to calculate theta and phi angles
/// to determine the position of the camera on the sphere.
/// </summary>
class Camera
{
	private:
		float mFov = 45.0f;

		// Center of the sphere on which the camera is moving
		glm::vec3 mPosition;
		glm::quat mRotation;

		glm::vec3 mForward;
		glm::vec3 mUp;
		glm::vec3 mRight;

		const glm::vec3 mWorldForward = glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//direction doesn't matter, only thing that matters is that it is indeed x-axis (both vec3(1.0f, 0.0f, 0.0f) and vec3(-1.0f, 0.0f, 0.0f) lie in x axis) - it actually does
		//matter because it will rotate differently when we rotate 180 around y axis => up becomes down and down becomes up
		const glm::vec3 mWorldRight = glm::vec3(1.0f, 0.0f, 0.0f);

		float mXOffset{};
		float mYOffset{};
		float mSphereRadius = 15.0f;
		glm::vec3 mPointOnSphere = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::mat4 view;
		glm::mat4 projection;

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
		
		//Modifies view and projection matrices and passes them to shader as uniforms
		void generateViewProjectionMatrices(Shader& shaderProgram);
		
		//Moves camera in all 3 axis
		void Move(GLFWwindow* window, float deltaTime);

		//Modifies fov to zoom in/out
		void Zoom(double amount);
		//Function for camera rotation
		//Rotation starts after pressing right mouse and ends when mouse gets released
		//By hovering mouse across screen (while right mouse is pressed), camera gets rotated
		void Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY);

		//Transforms 2D pixel coordinates to 3D world coordinates
		void ScreenToWorldCoordinates(const double mouseX, const double mouseY, glm::vec4& start, glm::vec3& direction);
		//Updates width and height parameters of camera to match width and height of viewport
		void UpdateViewportDimensions(const int& width, const int& height);

		//Turns x and y coordinate of a pixel into world position and casts a ray starting from that position
		//and going into direction of a camera
		void Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY);

		//Calculates point on sphere based off of x and y mouse offsets accumulated
		//through mouse movement
		void calculatePointOnSphere(const double& startingX, const double& startingY, 
			const double& currentX, const double& currentY);
		//Updates camera local axis based off of the point on sphere the camera is currently on
		void updateCameraAxis();
		void RestartCameraParameters();


		float alignOffset(float offset, float upBound);
		glm::vec3 GetCameraForward() { return mForward; };
		glm::vec3 GetCameraUp() { return mUp; };
		glm::vec3 GetCameraRight() { return mRight; };
		glm::vec3 GetCameraPosition() { return mPosition; };
		glm::quat GetCameraRotation() { return mRotation; };

		~Camera();
};