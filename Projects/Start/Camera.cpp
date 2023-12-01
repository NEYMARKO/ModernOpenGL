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

void Camera::ViewProjectionMatrix(glm::vec3 lookAtPoint, Shader& shaderProgram, Shader& lightShaderProgram)
{
	//glm::mat4 view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraForward, this->cameraUp);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = glm::lookAt(this->cameraPos, lookAtPoint, this->cameraUp);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
	unsigned int viewLocation = glGetUniformLocation(shaderProgram.ID, "view");
	unsigned int projectionLocation = glGetUniformLocation(shaderProgram.ID, "projection");
	
	unsigned int lightViewLocation = glGetUniformLocation(lightShaderProgram.ID, "view");
	unsigned int lightProjectionLocation = glGetUniformLocation(lightShaderProgram.ID, "projection");
	
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
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

void Camera::Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY)
{
	//TODO: CLAMP YAW AND PITCH VALUES: YAW [-90, 90] PITCH [-45, 45]

	int windowHeight, windowWidth;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	//amount which was traversed with cursor in x and y direction on screen
	float yaw = (float)(currentX - startingX) / windowWidth * this->sensitivity;
	float pitch = (float)(startingY - currentY) / windowHeight * this->sensitivity;

	this->cameraDirection = glm::rotate(this->cameraDirection, glm::radians(yaw), this->cameraUp);
	glm::vec3 cameraRight = glm::normalize(glm::cross(this->cameraUp, this->cameraDirection));
	this->cameraDirection = glm::rotate(this->cameraDirection, glm::radians(pitch), cameraRight);
	//after applying pitch rotation, cameraUp vector gets changed
	this->cameraUp = glm::cross(this->cameraDirection, cameraRight);
}