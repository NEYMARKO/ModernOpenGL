#include "CameraRotateState.h"

void CameraRotateState::onMouseMove(double x, double y)
{
	//std::cout << "STARTING Y: " << m_stateMachine->mousePosY << " CURRENT Y: " << y << "\n";
	m_camera->Rotate(m_stateMachine->mousePosX, m_stateMachine->mousePosY, x, y);
}

void CameraRotateState::enter()
{
	m_stateMachine->m_window->changeCursorShape(CursorShape::CLOSED_HAND_CURSOR);
}

void CameraRotateState::exit()
{
	m_stateMachine->m_window->changeCursorShape(CursorShape::DEFAULT_CURSOR);
}