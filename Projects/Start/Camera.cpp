#include "Camera.h"

#define ANGLE_LOWER_BOUND 10

Camera::Camera(glm::vec3 position, glm::vec3 targetPos, float speed, float sensitivity, int width, int height)
{
	this->position = position;
	this->forward = glm::normalize(targetPos - position);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float)width;
	this->height = (float)height;
	this->ray = nullptr;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	calculateCameraUp(worldUp);
}

Camera::Camera(glm::vec3 position, glm::vec3 targetPos, float speed, float sensitivity, glm::vec3 worldUp, int width, int height)
{
	this->position = position;
	this->forward = glm::normalize(targetPos - position);
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->lookAtPosition = targetPos;
	this->width = (float)width;
	this->height = (float)height;
	this->ray = nullptr;
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	calculateCameraUp(worldUp);
}

void Camera::calculateCameraUp(glm::vec3 worldUp)
{
	glm::vec3 worldUpNorm = glm::normalize(worldUp);
	this->right = glm::cross(this->forward, worldUpNorm);
	this->up = glm::cross(this->right, this->forward);
}

void Camera::ViewProjectionMatrix(Shader& shaderProgram)
{
	//If window gets minimized, it's width and height become 0 => when calculating projection matrix, 0 division happens
	if (this->width <= 0 || this->height <= 0)
	{
		return;
	}
	//glm::mat4 view = glm::lookAt(this->position, this->position + this->cameraForward, this->up);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(this->position, this->position + this->forward, this->up);
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
		this->position += cameraSpeed * this->forward;
		/*std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;*/

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->position -= cameraSpeed * this->forward;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->position += cameraSpeed * this->right;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->position -= cameraSpeed * this->right;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->position += cameraSpeed * this->up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		this->position -= cameraSpeed * this->up;
	}


	//glm::vec3 direction = this->position - this->targetPos;
	if (!this->focus)
	{
		this->lookAtPosition = this->forward + this->position;
	}
}

void Camera::Zoom(double amount)
{
	this->fov += amount * - 2;
}

void Camera::Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY)
{

	///https://gamedev.stackexchange.com/questions/30644/how-to-keep-my-quaternion-using-fps-camera-from-tilting-and-messing-up/30669#30669
	double xOffset = currentX - startingX;
	double yOffset = currentY - startingY;

	/*std::cout << "Y offset: " << yOffset << std::endl;
	std::cout << "X offset: " << xOffset << std::endl << std::endl;*/
	float yAngle = fabs(xOffset) <= ANGLE_LOWER_BOUND ? 0 : xOffset / (2 * this->width) * glm::radians(360.0f);
	float xAngle = fabs(yOffset) <= ANGLE_LOWER_BOUND ? 0 : yOffset / (2 * this->height) * glm::radians(360.0f);

	//yAngle = xOffset / (2 * this->width) * glm::radians(360.0f);
	/*std::cout << "Y angle (radians): " << yAngle << std::endl;
	yAngle = (2 * xOffset) / (this->width) * 360.0f;
	std::cout << "Y angle (degrees): " << yAngle << std::endl;*/


	//std::cout << "Y angle: " << yAngle << std::endl;
	//std::cout << "X angle: " << xAngle << std::endl << std::endl;

	// Rotate around the local y-axis using quaternions

	///CAMERA SHOULD BE ROTATED ALONG WORLD AXIS
	if (yAngle != 0)
	{
		glm::quat rotationY = glm::angleAxis(yAngle * this->sensitivity, this->up);
		this->forward = rotationY * this->forward;
		this->right = rotationY * this->right;
	}

	// Rotate around the local x-axis using quaternions
	if (xAngle != 0)
	{
		glm::quat rotationX = glm::angleAxis(-xAngle * this->sensitivity, this->right);
		this->forward = this->forward * rotationX;
		this->up = this->up * rotationX;
	}

	// Normalize vectors
	this->right = glm::normalize(this->right);
	this->up = glm::normalize(this->up);
	this->forward = glm::normalize(glm::cross(this->up, this->right));
}

void Camera::UpdateViewportDimensions(const int& width, const int& height)
{
	this->width = (float)width;
	this->height = (float)height;
}
void Camera::Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY)
{

	glm::vec4 rayStartWorld = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 rayDirectionWorld = glm::vec3(1.0f, 1.0f, 1.0f);
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
	glm::mat4 viewMatrix = glm::lookAt(this->position, this->lookAtPosition, this->up);

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

glm::vec3 Camera::GetCameraUp()
{
	return this->up;
}

glm::vec3 Camera::GetCameraRight()
{
	return this->right;
}

glm::vec3 Camera::GetCameraPosition()
{
	return this->position;
}
glm::quat Camera::GetCameraRotation()
{
	return this->rotation;
}

Camera::~Camera()
{
	std::cout << "Deleted camera" << std::endl;
}