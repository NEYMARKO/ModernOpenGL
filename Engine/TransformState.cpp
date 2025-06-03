#include <iostream>
#include "Camera.h"
#include "Object.h"
#include "Transform.h"
#include "TransformState.h"

#define EPSILON 0.001f

TransformState::TransformState(StateMachine* stateMachine, Camera* camera, Object* object) :
	State{ stateMachine }, m_camera{ camera },
	m_selectedObject{ object },
	m_transformPlane{ object->getComponent<Transform>()->getPosition() }
{ 
}

void TransformState::enter()
{
	glm::vec3 cameraNormal = m_camera->GetCameraForward();
	cameraNormal *= -1;
	m_transformPlane.calculatePlaneParameters(cameraNormal);
	m_transformPlane.calculateRayIntersectionPoint(mouseWorldPos, mouseWorldDirection);
	m_transforming = false;
}

void TransformState::onMouseClick(const glm::vec3& start, const glm::vec3& dir,
	int button, int action)
{
	if (!m_transforming)
	{
		std::cout << "Currently not transforming\n";
		State::onMouseClick(start, dir, button, action);
	}
	else
	{
		m_transforming = false;
		m_transformAxis = TransformAxis::NONE;
		//Finished transforming, stop tracking mouse and updating transform values
		std::cout << "FINISHED TRANSFORMING - CLICKED TO CONFIRM TRANSFORM\n";
	}
}
void TransformState::onKeyboardPress(const int keyCode)
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
	case GLFW_KEY_X:
		m_transformAxis = TransformAxis::X;
		break;
	case GLFW_KEY_Y:
		m_transformAxis = TransformAxis::Y;
		break;
	case GLFW_KEY_Z:
		m_transformAxis = TransformAxis::Z;
		break;
	case GLFW_KEY_F:
		toggleFreeMode();
		break;
	default:
		m_transitionState = States::DEFAULT;
	}
}

void TransformState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	glm::vec3 dir;
	switch (m_transformAxis)
	{
	case TransformAxis::X:
		dir = worldRight;
		break;
	case TransformAxis::Y:
		dir = worldUp;
		break;
	case TransformAxis::Z:
		dir = worldForward;
		break;
	}
	//direction vector isn't ZERO => it's transformation should be performed around axis
	//if (!glm::all(glm::lessThan(glm::abs(dir), glm::vec3(EPSILON))))
	//{
	//	//get ray position and direction in world space 
	//	m_camera->ScreenToWorldCoordinates(mouseX, mouseY,
	//		mouseWorldPos, mouseWorldDirection);
	//	m_transformPlane.projectVectorToPlane(dir);
	//	glm::vec3 infiniteAxis = (m_transformPlane.m_origin + 100.0f * dir)
	//		- (m_transformPlane.m_origin - 100.0f * dir);

	//}

}

void TransformState::update()
{
	//every movement of mouse should be tracked
	/*if (m_freeMode)
	{ 
		onMouseMove()
	}*/
}