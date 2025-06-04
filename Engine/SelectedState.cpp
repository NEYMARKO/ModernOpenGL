#include "SelectedState.h"
#define GLFW_INCLUDE_NONE	
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <iostream>
void SelectedState::onKeyboardPress(int key, int action)
{
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