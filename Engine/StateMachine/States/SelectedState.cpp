#include "../StateMachine.h"
#include "../../MeshRenderer.h"
#include "SelectedState.h"

#include <iostream>

void SelectedState::onKeyboardPress(int key, int action)
{
	State::onKeyboardPress(key, action);
	if (m_transitionState == States::CAMERA_MOVE)
		return;

	if (action != GLFW_PRESS)
		return;
	switch (key)
	{
	case GLFW_KEY_G:
		m_transitionState = States::GRAB;
		std::cout << "SELECTED => GRAB\n";
		break;
	case GLFW_KEY_R:
		m_transitionState = States::ROTATE;
		std::cout << "SELECTED => ROTATE\n";
		break;
	case GLFW_KEY_S:
		m_transitionState = States::SCALE;
		std::cout << "SELECTED => SCALE\n";
		break;
	default:
		m_transitionState = States::NO_TRANSITION;
		break;
	}
}