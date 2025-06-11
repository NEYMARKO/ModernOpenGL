#include "CameraMoveState.h"

void CameraMoveState::update()
{
	if (glm::all(glm::lessThan(glm::abs(m_moveDir), m_zeroVector)))
	{
		//std::cout << "NOT MOVING ANYMORE\n";
		return;
	}
	//std::cout << "IN CAMERA MOVE UPDATE\n";
	m_camera->Move(m_moveDir, 0.02f);
}

void CameraMoveState::updateMoveDirection(int key, int action)
{
	float direction;
	if (action == GLFW_PRESS)
		direction = 1;
	else if (action == GLFW_RELEASE)
		direction = -1;
	else
		return;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_moveDir += glm::vec3(0.0f, 0.0f, -1.0f) * direction;
		break;
	case GLFW_KEY_DOWN:
		m_moveDir -= glm::vec3(0.0f, 0.0f, -1.0f) * direction;
		break;
	case GLFW_KEY_RIGHT:
		m_moveDir += glm::vec3(1.0f, 0.0f, 0.0f) * direction;
		break;
	case GLFW_KEY_LEFT:
		m_moveDir -= glm::vec3(1.0f, 0.0f, 0.0f) * direction;
		break;
	case GLFW_KEY_SPACE:
		m_moveDir += glm::vec3(0.0f, 1.0f, 0.0f) * direction;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		m_moveDir -= glm::vec3(0.0f, 1.0f, 0.0f) * direction;
		break;
	default:
		break;
	}
}

void CameraMoveState::onKeyboardPress(int key, int action)
{

	if (key == GLFW_KEY_UP ||
		key == GLFW_KEY_DOWN ||
		key == GLFW_KEY_LEFT ||
		key == GLFW_KEY_RIGHT ||
		key == GLFW_KEY_SPACE ||
		key == GLFW_KEY_LEFT_CONTROL)
	{
		updateMoveDirection(key, action);
	}
	//std::cout << "MOVE DIR: (" << m_moveDir.x << ", " << m_moveDir.y << ", " << m_moveDir.z << ")\n";
	//if (action == GLFW_PRESS)
	//{
	//	if (key == GLFW_KEY_UP ||
	//		key == GLFW_KEY_DOWN ||
	//		key == GLFW_KEY_LEFT ||
	//		key == GLFW_KEY_RIGHT ||
	//		key == GLFW_KEY_SPACE ||
	//		key == GLFW_KEY_LEFT_CONTROL)
	//	{
	//		m_movingCamera = true;
	//		m_key = key;
	//	}
	//	else
	//	{
	//		std::cout << "NOT ONE OF THE KEYS\n";
	//		m_movingCamera = false;
	//		m_transitionState = States::NO_TRANSITION;
	//		m_key = GLFW_KEY_UNKNOWN;
	//	}
	//}
	////key was released
	//else if (action == GLFW_RELEASE)
	//{
	//	switch (key)
	//	{
	//	case GLFW_KEY_UP:
	//		m_moveDir -= glm::vec3(0.0f, 0.0f, -1.0f);
	//		break;
	//	case GLFW_KEY_DOWN:
	//		m_moveDir += glm::vec3(0.0f, 0.0f, -1.0f);
	//		break;
	//	case GLFW_KEY_RIGHT:
	//		m_moveDir -= glm::vec3(1.0f, 0.0f, 0.0f);
	//		break;
	//	case GLFW_KEY_LEFT:
	//		m_moveDir += glm::vec3(1.0f, 0.0f, 0.0f);
	//		break;
	//	case GLFW_KEY_SPACE:
	//		m_moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);
	//		break;
	//	case GLFW_KEY_LEFT_CONTROL:
	//		m_moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	//		break;
	//	default:
	//		break;
	//	}
	//	std::cout << "RELEASED KEY\n";
	//	m_movingCamera = false;
	//	m_transitionState = States::NO_TRANSITION;
	//	m_key = GLFW_KEY_UNKNOWN;
	//}
}