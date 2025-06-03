#include "SelectedState.h"
#define GLFW_INCLUDE_NONE	
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

void SelectedState::onKeyboardPress(int key)
{
	switch (key)
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
	default:
		m_transitionState = States::NO_TRANSITION;
		break;
	}
}