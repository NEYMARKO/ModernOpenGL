#include <iostream>
#include "Camera.h"
#include "Transform.h"
#include "TransformState.h"

#define EPSILON 0.001f

TransformState::TransformState(StateMachine* stateMachine, Camera* camera, Transform* objectTransform) :
	State{ stateMachine }, m_camera{ camera },
	m_selectedTransform{ objectTransform },
	m_transformPlane{ objectTransform->getPosition() }
{ 
}

void TransformState::enter()
{
	glm::vec3 cameraNormal = m_camera->GetCameraForward();
	cameraNormal *= -1;
	m_transformPlane.calculatePlaneParameters(cameraNormal);
	m_transformPlane.calculateRayIntersectionPoint(mouseWorldPos, mouseWorldDirection);
}

void TransformState::onMouseClick(const glm::vec3& start, const glm::vec3& dir,
	int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//std::cout << "MOUSE CLICK IN TRANSFORM\n";
		if (m_trackingMouse)
		{
			m_trackingMouse = false;
			m_transformAxis = TransformAxis::NONE;
			//Finished transforming, stop tracking mouse and updating transform values
			std::cout << "FINISHED TRANSFORMING - CLICKED TO CONFIRM TRANSFORM\n";
		}
		else
		{
			std::cout << "Currently not transforming\n";
			State::onMouseClick(start, dir, button, action);
		}
	}
}
void TransformState::onKeyboardPress(const int keyCode, int action)
{
	if (action != GLFW_PRESS)
		return;
	switch (keyCode)
	{
	case GLFW_KEY_G:
		m_transitionState = States::GRAB;
		std::cout << "TRANSFORM => GRAB\n";
		break;
	case GLFW_KEY_R:
		m_transitionState = States::ROTATE;
		std::cout << "TRANSFORM => ROTATE\n";
		break;
	case GLFW_KEY_S:
		m_transitionState = States::SCALE;
		std::cout << "TRANSFORM => SCALE\n";
		break;
	case GLFW_KEY_X:
		m_transformAxis = TransformAxis::X;
		std::cout << "TRANSFORM => X_AXIS\n";
		break;
	case GLFW_KEY_Y:
		m_transformAxis = TransformAxis::Y;
		std::cout << "TRANSFORM => Y_AXIS\n";
		break;
	case GLFW_KEY_Z:
		m_transformAxis = TransformAxis::Z;
		std::cout << "TRANSFORM => Z_AXIS\n";
		break;
	case GLFW_KEY_F:
		toggleFreeMode();
		break;
	default:
		m_transitionState = States::DEFAULT;
	}

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
	m_transformPlane.projectVectorToPlane(dir);
	m_infiniteAxis = (m_transformPlane.m_origin + dir * 100.0f)
		- (m_transformPlane.m_origin + dir * -100.0f);

}

void TransformState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	if (!m_trackingMouse)
	{
		//std::cout << "SHOULD CALL MOUSE MOVE FROM PARENT CLASS\n";
		State::onMouseMove(mouseStartWorld, mouseDirectionWorld);
		return;
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