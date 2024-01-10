#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 targetPos, float speed, float sensitivity, int width, int height)
{
	this->cameraPos = cameraPos;
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
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
	this->targetPos = targetPos;
	this->cameraDirection = glm::normalize(targetPos - cameraPos);
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
	//float normalizedX = (2.0f * mouseX) / this->width - 1.0f;
	//float normalizedY = 1.0f - (2.0f * mouseY) / this->height;

	//std::cout << "NORMALIZED X,Y: (" << normalizedX << ", " << normalizedY << ")" << std::endl;

	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);
	//glm::mat4 viewMatrix = glm::lookAt(this->cameraPos, this->lookAtPosition, this->cameraUp);

	//glm::vec4 homogenousPoint = glm::vec4(normalizedX, normalizedY, -1.0f, 1.0f);
	//glm::vec4 viewSpacePoint = glm::inverse(projectionMatrix) * homogenousPoint;
	//viewSpacePoint = glm::vec4(viewSpacePoint.x, viewSpacePoint.y, 0.0, 1.0);
	//std::cout << "VIEWSPACE X,Y,Z: (" << viewSpacePoint.x << ", " << viewSpacePoint.y << ", " << viewSpacePoint.z << ")" << std::endl;
	//glm::vec4 worldPoint = glm::inverse(viewMatrix) * viewSpacePoint;
	//// don't forget to normalise the vector at some point
	////ray_wor = glm::normalize(ray_wor);

	//std::cout << "WORLD X,Y,Z: (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")" << std::endl;
	//float d = -this->cameraPos.x * this->cameraDirection.x - this->cameraPos.y * this->cameraDirection.y - this->cameraPos.z * this->cameraDirection.z;
	//float ray_z = (-worldPoint.x * this->cameraDirection.x - worldPoint.y * this->cameraDirection.y - d) / this->cameraDirection.z;

	//worldPoint.z = ray_z;

	//if (this->ray == nullptr)
	//{
	//	this->ray = new Ray(worldPoint, this->cameraDirection, 1000);
	//}
	//else
	//{
	//	this->ray->UpdatePosition(worldPoint);
	//	this->ray->UpdateDirection(this->cameraDirection);
	//}
	////this->ray->Draw(shaderProgram, *this);

	float xNDC = (2.0f * mouseX) / this->width - 1.0f;
	float yNDC = 1.0f - (2.0f * mouseY) / this->height;
	/*std::cout << mouseX << std::endl;
	std::cout << mouseY << std::endl;

	std::cout << xNDC << std::endl;
	std::cout << yNDC << std::endl;*/

	glm::vec4 rayStartNDC = glm::vec4(xNDC, yNDC, -1.0, 1.0);

	glm::vec4 rayEndNDC = glm::vec4(xNDC, yNDC, 1.0, 1.0);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->fov), this->width / this->height, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(this->cameraPos, this->lookAtPosition, this->cameraUp);

	glm::mat4 invProjection = glm::inverse(projectionMatrix);
	glm::vec4 rayStartView = invProjection * rayStartNDC;
	glm::vec4 rayEndView = invProjection * rayEndNDC;

	/*glm::mat4 invView = glm::inverse(viewMatrix);
	glm::vec4 rayStartWorld = invView * rayStartView;
	glm::vec4 rayEndWorld = invView * rayEndWorld;*/
	rayStartView.w = -rayStartView.w;
	rayStartView /= rayStartView.w;
	rayEndView /= rayEndView.w;

	//std::cout << "START: " << rayStartWorld.x << " " << rayStartWorld.y << " " << rayStartWorld.z << std::endl;
	//std::cout << "END: " << rayEndWorld.x << " " << rayEndWorld.y << " " << rayEndWorld.z << std::endl;

	glm::vec3 rayDirectionWorld = glm::vec3(rayEndView - rayStartView);
	float length = glm::length(rayDirectionWorld);
	//glm::vec3 rayDirectionWorld = glm::vec3(rayEndWorld) - this->cameraPos;
	rayDirectionWorld = glm::normalize(rayDirectionWorld);


	/*float d = -this->cameraPos.x * this->cameraDirection.x - this->cameraPos.y * this->cameraDirection.y - this->cameraPos.z * this->cameraDirection.z;
	float ray_z = (-rayEndWorld.x * this->cameraDirection.x - rayEndWorld.y * this->cameraDirection.y - d) / this->cameraDirection.z;

	std::cout << "NEW Z: " << ray_z << std::endl;
	std::cout << this->cameraPos.z << std::endl;
	rayStartWorld.z = ray_z;*/

	//std::cout << rayStartWorld.x << " " << rayStartWorld.y << " " << rayStartWorld.z << std::endl;
	//std::cout << this->cameraPos.x << " " << this->cameraPos.y << " " << this->cameraPos.z << std::endl;
	if (this->ray == nullptr)
	{
		//this->ray = new Ray(this->cameraPos, rayDirectionWorld, 1000);
		this->ray = new Ray(this->cameraPos, rayDirectionWorld, length);
	}
	else
	{
		delete this->ray;
		//this->ray = new Ray(this->cameraPos, rayDirectionWorld, 1000);
		this->ray = new Ray(this->cameraPos, rayDirectionWorld, length);
	}
	//this->ray->Draw(shaderProgram, *this);

}