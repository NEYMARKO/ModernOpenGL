#include <iostream>
#include "../../Rendering/Camera.h"
#include "../../Components/Transform.h"
#include "../../Object.h"
#include "../../Components/EditorCollider.h"
#include "../StateMachine.h"
#include "../../Physics/PhysicsWorld.h"
#include "./TransformState.h"

#ifndef EC_ENABLE_VISUAL_DEBUG
#define EC_ENABLE_VISUAL_DEBUG
#endif

#define EPSILON 0.001f

TransformState::TransformState(StateMachine* stateMachine, Camera* camera, Transform* objectTransform) :
	State{ stateMachine, true }, m_camera{ camera },
	m_selectedTransform{ objectTransform },
	m_transformPlane{ objectTransform->getPosition() }/*, 
	m_mouseStart { glm::vec3(m_stateMachine->mouseStartWorld) }*/
{ 
	glm::vec3 cameraNormal = m_camera->GetCameraForward();
	cameraNormal *= -1;
	m_transformPlane.calculatePlaneParameters(cameraNormal);
	//m_transformPlane.calculateRayIntersectionPoint(mouseWorldPos, mouseWorldDirection);
	if (!m_stateMachine->m_target)
		std::cout << "TARGET IS NULL\n";
#ifdef EC_ENABLE_VISUAL_DEBUG
	m_stateMachine->m_target->m_editorCollider.setupAABB();
#endif
	/*m_mouseStart = m_transformPlane.calculateRayIntersectionPoint(m_stateMachine->mouseStartWorld,
		m_stateMachine->mouseDirectionWorld);*/
}

TransformState::TransformState(StateMachine* stateMachine, Camera* camera, const btVector3& pos) :
	State{ stateMachine, true }, m_camera{ camera }, 
	m_transformPlane { glm::vec3(pos.getX(), pos.getY(), pos.getZ()) }
{
	glm::vec3 cameraNormal = m_camera->GetCameraForward();
	cameraNormal *= -1;
	m_transformPlane.calculatePlaneParameters(cameraNormal);
}

void TransformState::enter()
{
	if (m_stateMachine->m_rbTarget)
	{
		m_rbPicker.m_dynamicsWorld = m_stateMachine->m_physicsWorld->getDynamicsWorld();
		m_rbPicker.m_pickedBody = m_stateMachine->m_rbTarget;
		m_rbPicker.m_pickPos = m_stateMachine->m_rbPickPos;
		glm::vec3 start = m_stateMachine->mouseStartWorld;
		glm::vec3 end = start + m_stateMachine->mouseDirectionWorld * m_stateMachine->m_rayLen;
		btVector3 rayFrom = btVector3(start.x, start.y, start.z);
		btVector3 rayTo = btVector3(end.x, end.y, end.z);
		m_rbPicker.pickBody(rayFrom, rayTo);
	}
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
			if (m_stateMachine->m_rbTarget)
			{
				m_rbPicker.removePickingConstraint();
			}
		}
		else
		{
			std::cout << "Currently not transforming\n";
			State::onMouseClick(start, dir, button, action);
		}
	}
	else if (GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		State::onMouseClick(glm::vec3(), glm::vec3(), button, action);
	}
}
void TransformState::onKeyboardPress(const int keyCode, int action)
{
	if (!m_trackingMouse)
	{
		State::onKeyboardPress(keyCode, action);
		if (m_transitionState == CAMERA_MOVE)
			return;
	}
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

	switch (m_transformAxis)
	{
	case TransformAxis::X:
		m_projectedAxis = m_worldRight;
		m_infiniteAxis = m_worldRight;
		break;
	case TransformAxis::Y:
		m_projectedAxis = m_worldUp;
		m_infiniteAxis = m_worldUp;
		break;
	case TransformAxis::Z:
		m_projectedAxis = m_worldForward;
		m_infiniteAxis = m_worldForward;
		break;
	}

	//Points get projected to projectedAxis (axis that was projected to plane).
	//Distance between origin of the plane and point projected to projectedAxis is calculated
	//and used to determine amount of translation object needs to perform along World axis.
	//Since projectedAxis spans from plane origin in both ways, it goes through origin (which is constant
	//as long as plane parameters don't need to be recalculated - it can be used to determine distance
	//that object should travel along world axis (distance from origin (which is constant) to projection of
	//plane intersection onto projectedAxis)
	m_transformPlane.projectVectorToPlane(m_projectedAxis);
	m_projectedAxis = (m_transformPlane.m_origin + m_projectedAxis * 100.0f)
		- (m_transformPlane.m_origin + m_projectedAxis * -100.0f);

}

void TransformState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	if (!m_trackingMouse)
	{
		//std::cout << "SHOULD CALL MOUSE MOVE FROM PARENT CLASS\n";
		State::onMouseMove(mouseStartWorld, mouseDirectionWorld);
		return;
	}

	//THIS WILL GET TRIGGERED ONLY ONCE BECAUSE GRAB AND ROTATE HAVE OVERRIDEN
	//onMouseMove functions => this is enough to have precise control over 
	//ray-box intersection, but will not provide visually satisfying result
	//since it gets updated only once per transformation
#ifdef EC_ENABLE_VISUAL_DEBUG
	if (m_stateMachine->m_target)
		m_stateMachine->m_target->m_editorCollider.setupAABB();
#endif
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
	if (m_stateMachine->m_target && m_trackingMouse)
		m_stateMachine->m_target->m_editorCollider.setupAABB();
}