#include "Camera.h"

#define ANGLE_LOWER_BOUND 10

Camera::Camera(glm::vec3 position, glm::vec3 targetPos, float speed, 
	float sensitivity, int width, int height, 
	const glm::vec3& worldUp)
	:
	mPosition { position }, mRotation { glm::quat(1.0f, 0.0f, 0.0f, 0.0f) }, 
	mForward { glm::normalize(targetPos - position) },
	mWorldUp { worldUp }, mWidth{ (float)width }, mHeight{ (float)height },
	mLookAtPosition{ targetPos }, mSpeed{ speed }, mSensitivity{ sensitivity },
	mRay { nullptr }
{
	//theta and phi have to be pi/2 to get sphere coordinate (0, 0, r) which is facing forward
	calculatePointOnSphere(0.0, 0.0, mWidth / (mSensitivity * 4), mHeight / (mSensitivity * 2));
	updateCameraAxis();
}

void Camera::CalculateCameraUp(glm::vec3 worldUp)
{
	glm::vec3 worldUpNorm = glm::normalize(worldUp);
	mRight = glm::cross(mForward, worldUpNorm);
	mUp = glm::cross(mRight, mForward);
}

void Camera::generateViewProjectionMatrices(Shader& shaderProgram)
{
	//If window gets minimized, it's width and height become 0 => when calculating projection matrix, 0 division happens
	if (mWidth <= 0 || mHeight <= 0)
	{
		return;
	}

	view = glm::lookAt(mPosition + mPointOnSphere, mPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(mFov), mWidth / mHeight, 0.1f, 100.0f);

	shaderProgram.SetMat4("view", view);
	shaderProgram.SetMat4("projection", projection);
}

//Good orientation 
//Camera is converging to negative x when Right key is pressed because camera is positioned behind the origin of the world
//Draw (0, 0, 0) and your camera at (0, 0, 5) and (0, 0, -5) and check up and right for those cases - everything works fine and makes sense

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = mSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mPosition += cameraSpeed * mForward;
		/*std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;*/

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mPosition -= cameraSpeed * mForward;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mPosition += cameraSpeed * mRight;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mPosition -= cameraSpeed * mRight;
		//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mPosition += cameraSpeed * mUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mPosition -= cameraSpeed * mUp;
	}

	//std::cout << "Camera position: " << mPosition.x << " " << mPosition.y << " " << mPosition.z << std::endl;

	//glm::vec3 direction = this->position - this->targetPos;
	if (!this->focus)
	{
		mLookAtPosition = mForward + mPosition;
	}
}

void Camera::Zoom(double amount)
{
	mFov += amount * - 2;
}

void Camera::Rotate(GLFWwindow* window, double startingX, double startingY, double currentX, double currentY)
{
	calculatePointOnSphere(startingX, startingY, currentX, currentY);
	updateCameraAxis();
}

void Camera::calculatePointOnSphere(const double& startingX, const double& startingY, const double& currentX, const double& currentY)
{
	mXOffset += (currentX - startingX);
	mYOffset += currentY - startingY;

	//angle around y-axis
	//should be in range [0,2pi]
	float theta = mXOffset / mWidth * glm::radians(360.0f) * mSensitivity;

	//angle around x-axis
	//should be in range [0,pi]
	float phi = mYOffset / mHeight * glm::radians(180.0f) * mSensitivity;

	mPointOnSphere.x = mSphereRadius * glm::cos(theta) * glm::sin(phi);
	mPointOnSphere.y = mSphereRadius * glm::cos(phi);
	mPointOnSphere.z = mSphereRadius * glm::sin(theta) * glm::sin(phi);
}

float Camera::alignOffset(float offset, float upBound)
{
	if (offset > upBound) return offset - 2 * upBound;
	else if (offset < -upBound) return offset + 2 * upBound;
	else return offset;
}
void Camera::updateCameraAxis()
{
	//RIGHT AXIS IS GETTING FLIPPED WHEN CAMERA IS UPSIDE DOWN
	//IF IT DIDN'T GET FLIPPED, LEFT ARROW KEY WOULD MOVE CAMERA TO THE RIGHT
	//AND RIGHT ARROW KEY WOULD MOVE CAMERA TO THE LEFT\
	//THE PROBLEM IS THAT THERE IS SNAPPING WHEN CAMERA PASSES k*PI ANGLES BECASUE OF MIRRORING (FLIPPED RIGHT AXIS)
	//vector pointing from A to B: B - A => center - eye
	mForward = glm::normalize(-mPointOnSphere);
	//std::cout << "FORWARD: " << msForward.x << " " << mForward.y << " " << mForward.z << std::endl;
	mRight = glm::normalize(glm::cross(mForward, mWorldUp));
	//std::cout << "RIGHT: " << mRight.x << " " << mRight.y << " " << mRight.z << std::endl;
	mUp = glm::normalize(glm::cross(mRight, mForward));
}

void Camera::UpdateViewportDimensions(const int& width, const int& height)
{
	float xRatio = (float)width / mWidth;
	float yRatio = (float)height / mHeight;

	mXOffset *= xRatio;
	mYOffset *= yRatio;

	mWidth = (float)width;
	mHeight = (float)height;

	calculatePointOnSphere(0.0, 0.0, 0.0, 0.0);
	updateCameraAxis();
}
void Camera::Raycast(GLFWwindow* window, const double& mouseX, const double& mouseY)
{

	glm::vec4 rayStartWorld = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 rayDirectionWorld = glm::vec3(1.0f, 1.0f, 1.0f);
	ScreenToWorldCoordinates(mouseX, mouseY, rayStartWorld, rayDirectionWorld);

	if (mRay == nullptr)
	{
		mRay = new Ray(rayStartWorld, rayDirectionWorld, 100);
	}
	else
	{
		delete mRay;
		mRay = new Ray(rayStartWorld, rayDirectionWorld, 100);
	}
}

void Camera::ScreenToWorldCoordinates(const double mouseX, const double mouseY, glm::vec4& rayStart, glm::vec3& rayDirection)
{

	float xNDC = (2.0f * mouseX) / mWidth - 1.0f;
	float yNDC = 1.0f - (2.0f * mouseY) / mHeight;

	rayStart = glm::vec4(xNDC, yNDC, 0.0, 1.0);
	glm::vec4 rayEnd = glm::vec4(xNDC, yNDC, 1.0, 1.0);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeight, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(mPosition + mPointOnSphere, mPosition, mUp);

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

void Camera::RestartCameraParameters()
{
	mPosition = glm::vec3(0.0f, 0.0f, -7.5f);
	mForward = glm::vec3(0.0f, 0.0f, 1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::cross(mForward, mUp);
}

Camera::~Camera()
{
	std::cout << "Deleted camera" << std::endl;
}