#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, int width, int height)
{
	this->cameraPos = cameraPos;
	this->forward = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float)width;
	this->height = (float)height;
	this->ray = nullptr;
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	calculateCameraUp(upVector);
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 upVector, int width, int height)
{
	this->cameraPos = cameraPos;
	this->forward = glm::normalize(targetPos - cameraPos);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float)width;
	this->height = (float)height;
	this->ray = nullptr;
	calculateCameraUp(upVector);
}

void Camera::calculateCameraUp(glm::vec3 upVector)
{
	glm::vec3 upVectorNorm = glm::normalize(upVector);
	this->right = glm::cross(this->forward, upVectorNorm);
	this->up = glm::cross(this->right, this->forward);
}

void Camera::ViewProjectionMatrix(Shader& shaderProgram)
{
	//If window gets minimized, it's width and height become 0 => when calculating projection matrix, 0 division happens
	if (this->width <= 0 || this->height <= 0)
	{
		return;
	}
	//glm::mat4 view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraForward, this->up);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(this->cameraPos, this->lookAtPosition, this->up);
	projection = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);

	shaderProgram.SetMat4("view", view);
	shaderProgram.SetMat4("projection", projection);
}

//Good orientation 
//Camera is converging to negative x when Right key is pressed because camera is positioned behind the origin of the world
//Draw (0, 0, 0) and your camera at (0, 0, 5) and (0, 0, -5) and check up and right for those cases - everything works fine and makes sense

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = this->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->forward;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->forward;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->right;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->right;
		//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->cameraPos += cameraSpeed * this->up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		this->cameraPos -= cameraSpeed * this->up;
	}


	//glm::vec3 direction = this->cameraPos - this->targetPos;
	if (!this->focus)
	{
		this->lookAtPosition = this->forward + this->cameraPos;
	}
}

void Camera::Zoom(double amount)
{
	this->fov += amount * - 2;
}

void Camera::Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY)
{
	//TODO: CLAMP YAW AND PITCH VALUES: YAW [-90, 90] PITCH [-45, 45]

	int windowHeight, windowWidth;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	//amount which was traversed with cursor in x and y direction on screen
	float yaw = (float)(currentX - startingX) / windowWidth * this->sensitivity;
	float pitch = (float)(startingY - currentY) / windowHeight * this->sensitivity;

	this->forward = glm::rotate(this->forward, glm::radians(yaw), this->up);
	//glm::vec3 right = glm::normalize(glm::cross(this->up, this->forward));
	this->right = glm::rotate(this->right, glm::radians(yaw), this->up);

	glm::normalize(this->forward);
	glm::normalize(this->right);

	this->forward = glm::rotate(this->forward, glm::radians(-pitch), this->right);
	//after applying pitch rotation, up vector gets changed
	//this->up = glm::cross(this->forward, right);
	this->up = glm::rotate(this->up, glm::radians(-pitch), this->right);
	glm::normalize(this->forward);
	glm::normalize(this->up);
}

void Camera::UpdateViewportDimensions(const int& width, const int& height)
{
	this->width = (float)width;
	this->height = (float)height;
}
void Camera::Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY)
{

	glm::vec4 rayStartWorld = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glm::vec3 rayDirectionWorld = glm::vec3(1.0, 1.0, 1.0);
	ScreenToWorldCoordinates(mouseX, mouseY, rayStartWorld, rayDirectionWorld);

	if (this->ray == nullptr)
	{
		this->ray = new Ray(rayStartWorld, rayDirectionWorld, 50);
	}
	else
	{
		delete this->ray;
		this->ray = new Ray(rayStartWorld, rayDirectionWorld, 50);
	}
}

void Camera::ScreenToWorldCoordinates(const double mouseX, const double mouseY, glm::vec4& rayStart, glm::vec3& rayDirection)
{

	float xNDC = (2.0f * mouseX) / this->width - 1.0f;
	float yNDC = 1.0f - (2.0f * mouseY) / this->height;

	rayStart = glm::vec4(xNDC, yNDC, 0.0, 1.0);
	glm::vec4 rayEnd = glm::vec4(xNDC, yNDC, 1.0, 1.0);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(this->cameraPos, this->lookAtPosition, this->up);

	glm::mat4 invProjection = glm::inverse(projectionMatrix);
	rayStart = invProjection * rayStart;
	rayEnd = invProjection * rayEnd;

	glm::mat4 invView = glm::inverse(viewMatrix);
	rayStart = invView * rayStart;
	rayEnd = invView * rayEnd;
	rayStart /= rayStart.w;
	rayEnd /= rayEnd.w;

	rayDirection = glm::vec3(rayEnd - rayStart);
	rayDirection = glm::normalize(rayDirection);
}

glm::vec3 Camera::GetCameraForward()
{
	return this->forward;
}
Camera::~Camera()
{
	std::cout << "Deleted camera" << std::endl;
}