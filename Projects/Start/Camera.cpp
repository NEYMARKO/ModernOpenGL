#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	calculateCameraUp();
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->upVector = upVector;
	this->lookAtPosition = targetPos;
	calculateCameraUp();
}

void Camera::calculateCameraUp()
{
	glm::vec3 upVectorNorm = glm::normalize(this->upVector);
	glm::vec3 cameraRightNorm = glm::cross(upVectorNorm, this->cameraDirection);
	this->cameraUp = glm::cross(this->cameraDirection, cameraRightNorm);
}

glm::mat4 Camera::LookAt(glm::vec3 lookAtPoint)
{
	//glm::mat4 view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraForward, this->cameraUp);
	glm::mat4 view = glm::lookAt(this->cameraPos, lookAtPoint, this->cameraUp);
	return view;
}

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = this->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->cameraDirection;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->cameraDirection;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		/*std::cout << "CAMERA position: (" << this->cameraPos.x << ", " <<
			this->cameraPos.y << ", " << this->cameraPos.z << ")" << std::endl;
		std::cout << "CAMERA IS LOOKING AT: (" << this->lookAtPosition.x << ", " <<
			this->lookAtPosition.y << ", " << this->lookAtPosition.z << ")" << std::endl;*/
		this->cameraPos += cameraSpeed * glm::cross(this->cameraDirection, this->cameraUp);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * glm::cross(this->cameraDirection, this->cameraUp);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->cameraUp;
	}

	//glm::vec3 direction = this->cameraPos - this->targetPos;
	if (!this->focus)
	{
		this->lookAtPosition = this->cameraDirection + this->cameraPos;
	}
}

//TODO: CLAMP YAW AND PITCH VALUES: YAW [-90, 90] PITCH [-45, 45]
void Camera::Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY)
{
	int windowHeight, windowWidth;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	//amount which was traversed with cursor in x and y direction on screen
	float yaw = (float)(currentX - startingX) / windowWidth * this->sensitivity;
	float pitch = (float)(startingY - currentY) / windowHeight * this->sensitivity;

	std::cout << "YAW: " << yaw << std::endl;
	this->totalYaw += (yaw / this->sensitivity);
	this->totalPitch += (pitch / this->sensitivity);

	this->cameraDirection = glm::rotate(this->cameraDirection, glm::radians(yaw), this->cameraUp);
	glm::vec3 cameraRight = glm::normalize(glm::cross(this->cameraUp, this->cameraDirection));
	this->cameraDirection = glm::rotate(this->cameraDirection, glm::radians(pitch), cameraRight);
	this->cameraUp = glm::cross(this->cameraDirection, cameraRight);
}