#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->upVector = upVector;
}

glm::mat4 Camera::LookAt(glm::vec3 targetPos)
{
	glm::mat4 view = glm::lookAt(this->cameraPos, this->targetPos, this->upVector);
	return view;
}

void Camera::Move(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 v = this->cameraPos - this->targetPos;
		std::cout << "DIRECTION: (" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
		std::cout << "LENGTH: " << glm::length(v) << std::endl;
		if (glm::length(this->cameraPos - this->targetPos) >= 0.75f)
		{
			this->cameraPos -= this->speed * this->cameraDirection;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->cameraPos += this->speed * this->cameraDirection;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->cameraPos += this->speed * glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->cameraPos += this->speed * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec3 direction = this->cameraPos - this->targetPos;
	//this->cameraDirection = this->cameraPos[0] < this->targetPos[0] ? 
		//glm::normalize(direction) : glm::normalize(-direction);
	this->cameraDirection = direction;


}