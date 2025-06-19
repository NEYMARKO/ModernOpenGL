#pragma once
#include "Shader.h"
//#include "OpenGLIncludes.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE
#include <glm/gtc/type_ptr.hpp>

class Ray;

/// <summary>
/// Camera is moving on the surface of the sphere. It is oriented in a way to always 
/// face the center of the sphere. Mouse position is being accumulated in offset variables 
/// when right mouse button is pressed. Those values are used to calculate theta and phi angles
/// to determine the position of the camera on the sphere.
/// </summary>
class Camera
{
	private:
		float m_fov = 45.0f;

		glm::vec3 m_position;
		glm::quat m_rotation;

		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;

		const glm::vec3 mWorldForward = glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//direction doesn't matter, only thing that matters is that it is indeed x-axis (both vec3(1.0f, 0.0f, 0.0f) and vec3(-1.0f, 0.0f, 0.0f) lie in x axis) - it actually does
		//matter because it will rotate differently when we rotate 180 around y axis => up becomes down and down becomes up
		const glm::vec3 mWorldRight = glm::vec3(1.0f, 0.0f, 0.0f);

		float mSphereRadius = 15.0f;
		glm::vec3 m_pivot;

		glm::mat4 m_view;
		glm::mat4 m_projection;

		float m_theta, m_phi;

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
		
		//Moves camera based off of direction that was passed as an argument. 
		//Multiplies direction vector with camera rotation to translate direction to 
		//camera's local space (relevant to the camera's local axis)
		void Move(glm::vec3 direction, float deltaTime);

		//Modifies fov to zoom in/out
		void Zoom(double amount);
		//Rotates camera while RM is being held. Uses difference between current and last position of mouse coordinates
		//to calculate yaw and pitch values. Rotates camera using arcball technique
		void Rotate(double startingX, double startingY, double currentX, double currentY);

		//Transforms 2D pixel coordinates to 3D world coordinates
		void ScreenToWorldCoordinates(const double mouseX, const double mouseY, glm::vec4& start, glm::vec3& direction);
		//Updates width and height parameters of camera to match width and height of viewport
		void UpdateViewportDimensions(const int& width, const int& height);

		//Turns x and y coordinate of a pixel into world position and casts a ray starting from that position
		//and going into direction of a camera
		void Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY);

		//Updates camera's local axis
		void updateCameraAxis();
		void RestartCameraParameters();

		glm::vec3 GetCameraForward() { return m_forward; };
		glm::vec3 GetCameraUp() { return m_up; };
		glm::vec3 GetCameraRight() { return m_right; };
		glm::vec3 GetCameraPosition() { return m_position; };
		glm::quat GetCameraRotation() { return m_rotation; };

		~Camera();
};