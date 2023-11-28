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

void Camera::Move(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->cameraPos += this->speed * this->cameraDirection;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->cameraPos -= this->speed * this->cameraDirection;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		/*std::cout << "CAMERA position: (" << this->cameraPos.x << ", " <<
			this->cameraPos.y << ", " << this->cameraPos.z << ")" << std::endl;
		std::cout << "CAMERA IS LOOKING AT: (" << this->lookAtPosition.x << ", " <<
			this->lookAtPosition.y << ", " << this->lookAtPosition.z << ")" << std::endl;*/
		this->cameraPos += this->speed * glm::cross(this->cameraDirection, this->cameraUp);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->cameraPos -= this->speed * glm::cross(this->cameraDirection, this->cameraUp);
	}

	//glm::vec3 direction = this->cameraPos - this->targetPos;
	if (!this->focus)
	{
		this->lookAtPosition = this->cameraDirection + this->cameraPos;
	}
}
void Camera::Rotate(GLFWwindow* window)
{
	if (GLFW_MOUSE_BUTTON_RIGHT && GLFW_PRESS)
	{
			
	}
}