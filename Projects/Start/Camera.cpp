#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, int width, int height)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float) width;
	this->height = (float) height;
	this->ray = nullptr;
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	calculateCameraUp(upVector);
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector, int width, int height)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float) width;
	this->height = (float) height;
	this->ray = nullptr;
	calculateCameraUp(upVector);
}

void Camera::calculateCameraUp(glm::vec3 upVector)
{
	glm::vec3 upVectorNorm = glm::normalize(upVector);
	glm::vec3 cameraRightNorm = glm::cross(this->cameraDirection, upVectorNorm);
	this->cameraUp = glm::cross(cameraRightNorm, this->cameraDirection);
}

void Camera::ViewProjectionMatrix(Shader& shaderProgram)
{
	//glm::mat4 view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraForward, this->cameraUp);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = glm::lookAt(this->cameraPos, this->lookAtPosition, this->cameraUp);
	//width/height instead of harcoded values (800/800)
	projection = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);

	shaderProgram.SetMat4("view", view);
	shaderProgram.SetMat4("projection", projection);
}

//Good orientation 
//Camera is converging to negative x when Right key is pressed because camera is positioned behind the origin of the world
//Draw (0, 0, 0) and your camera at (0, 0, 5) and (0, 0, -5) and check cameraUp and cameraRight for those cases - everything works fine and makes sense

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = this->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->cameraDirection;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->cameraDirection;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * glm::cross(this->cameraDirection, this->cameraUp);
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * glm::cross(this->cameraDirection, this->cameraUp);
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
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

void Camera::Zoom(double amount)
{
	this->fov += amount * 2;
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

void Camera::UpdateViewportDimensions(const int& width, const int& height)
{
	this->width = (float)width;
	this->height = (float)height;
}
void Camera::Raycast(GLFWwindow* window, Shader& shaderProgram, double mouseX, double mouseY)
{
	//normalized device coordinates
	float x = (2.0f * mouseX) / width - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / height;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(this->cameraPos, this->lookAtPosition, this->cameraUp);

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	
	glm::vec3 ray_wor = glm::vec3(glm::inverse(viewMatrix) * ray_eye);
	// don't forget to normalise the vector at some point
	//ray_wor = glm::normalize(ray_wor);

	float d = -this->cameraPos.x * this->cameraDirection.x - this->cameraPos.y * this->cameraDirection.y - this->cameraPos.z * this->cameraDirection.z;
	float ray_z = (-ray_wor.x * this->cameraDirection.x - ray_wor.y * this->cameraDirection.y - d) / this->cameraDirection.z;

	ray_wor.z = ray_z;
	if (this->ray == nullptr)
	{
		this->ray = new Ray(ray_wor, this->cameraDirection, 500);
	}
	else
	{
		this->ray->UpdatePosition(ray_wor);
		this->ray->UpdateDirection(this->cameraDirection);
	}
	//this->ray->Draw(shaderProgram, *this);
}