#include "Camera.h"
#include "Object.h"
#include "Transform.h"
#include "TransformState.h"

void TransformPlane::calculatePlaneParameters(Camera* camera, Object* target)
{
	normal = -camera->GetCameraForward();
	D = -glm::dot(normal, target->getComponent<Transform>()->getPosition());
}
void TransformPlane::updateIntersectionPoint(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	float t;
	t = (-glm::dot(normal, glm::vec3(mouseStartWorld)) - D) / glm::dot(normal, mouseDirectionWorld);
	intersectionPoint = glm::vec3(mouseStartWorld) + mouseDirectionWorld * t;
}


void TransformState::enter()
{
	m_transformPlane.calculatePlaneParameters(m_camera, m_selectedObject);
	m_transformPlane.updateIntersectionPoint(mouseWorldPos, mouseWorldDirection);
}

void TransformState::onKeyboardPress(const int keyCode, const int action)
{
	switch (keyCode)
	{
	case GLFW_KEY_G:
		m_transitionState = States::GRAB;
		break;
	case GLFW_KEY_R:
		m_transitionState = States::ROTATE;
		break;
	case GLFW_KEY_S:
		m_transitionState = States::SCALE;
		break;
	case GLFW_KEY_F:
		toggleFreeMode();
		break;
	default:
		m_transitionState = States::DEFAULT;
	}
}

void TransformState::onMouseMove(const double mouseX, const double mouseY)
{
	m_camera->ScreenToWorldCoordinates(mouseX, mouseY,
		mouseWorldPos, mouseWorldDirection);
}

void TransformState::update()
{
	//every movement of mouse should be tracked
	/*if (m_freeMode)
	{ 
		onMouseMove()
	}*/
}