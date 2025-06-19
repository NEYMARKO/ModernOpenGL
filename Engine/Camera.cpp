#include "Ray.h"
#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>


Camera::Camera(glm::vec3 position, glm::vec3 targetPos, float speed, 
	float sensitivity, int width, int height, 
	const glm::vec3& worldUp)
	:
	m_position { position }, m_rotation { glm::quat(1.0f, 0.0f, 0.0f, 0.0f) }, 
	m_forward { /*glm::normalize(targetPos - position)*/ glm::vec3(0,0,-1.0f) },
	mWorldUp { worldUp }, mWidth{ (float)width }, mHeight{ (float)height },
	mLookAtPosition{ targetPos }, mSpeed{ speed }, mSensitivity{ sensitivity },
	mRay { nullptr }, m_pivot{ m_position + m_forward * mSphereRadius }
{
	updateCameraAxis();
}

void Camera::CalculateCameraUp(glm::vec3 worldUp)
{
	glm::vec3 worldUpNorm = glm::normalize(worldUp);
	m_right = glm::cross(m_forward, worldUpNorm);
	m_up = glm::cross(m_right, m_forward);
}

void Camera::generateViewProjectionMatrices(Shader& shaderProgram)
{
	//If window gets minimized, it's width and height become 0 => when calculating projection matrix, 0 division happens
	if (mWidth <= 0 || mHeight <= 0)
	{
		return;
	}

	m_view = glm::lookAt(m_position, m_pivot, m_up);
	m_projection = glm::perspective(glm::radians(m_fov), mWidth / mHeight, 0.1f, 100.0f);

	shaderProgram.SetMat4("view", m_view);
	shaderProgram.SetMat4("projection", m_projection);
}

void Camera::Move(glm::vec3 direction, float deltaTime)
{
	direction = m_rotation * glm::normalize(direction);
	float cameraSpeed = mSpeed * deltaTime;
	m_position += direction * cameraSpeed;
	m_pivot += direction * cameraSpeed;

	if (!this->focus)
	{
		mLookAtPosition = m_forward + m_position;
	}
}

void Camera::Zoom(double amount)
{
	m_fov += amount * - 2;
}

void Camera::Rotate(double startingX, double startingY, double currentX, double currentY)
{
	float pitch = (currentY - startingY) / mWidth * glm::radians(360.0f) * mSensitivity/2;
	//invert yaw controls
	float yaw = (currentX - startingX) / mHeight * glm::radians(180.0f) * mSensitivity * -1;

	glm::quat q_yaw = glm::angleAxis(yaw, mWorldUp);
	glm::vec3 right = glm::normalize(m_rotation * mWorldRight);
	glm::quat q_pitch = glm::angleAxis(pitch, right);
	glm::quat delta_rotation = glm::normalize(q_yaw * q_pitch);

	m_rotation = glm::normalize(delta_rotation * m_rotation);

	m_position = delta_rotation * (m_position - m_pivot) + m_pivot;

	updateCameraAxis();
}


void Camera::updateCameraAxis()
{
	m_forward = glm::normalize(m_pivot - m_position);
	m_right = glm::normalize(m_rotation * mWorldRight);
	m_up = glm::normalize(m_rotation * mWorldUp);

	//m_rotation = glm::quat_cast(glm::inverse(view));
}

void Camera::UpdateViewportDimensions(const int& width, const int& height)
{
	if (width <= 0 || height <= 0)
		return;

	mWidth = (float)width;
	mHeight = (float)height;
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

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_fov), mWidth / mHeight, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(m_position, m_pivot, m_up);

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
	m_position = glm::vec3(0.0f, 0.0f, -7.5f);
	m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::cross(m_forward, m_up);
}

Camera::~Camera()
{
	std::cout << "Deleted camera" << std::endl;
}