#include "CameraRotateState.h"

void CameraRotateState::onMouseMove(double x, double y)
{
	m_camera->Rotate(m_stateMachine->mousePosX, m_stateMachine->mousePosY, x, y);
}