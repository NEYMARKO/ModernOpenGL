#pragma once
#include "StateMachine.h"
#include "Camera.h"
#include "State.h"
#include "Lighting.h"
#include "Transform.h"

class CameraRotateState : public State
{
private:
	Camera* m_camera;
	Transform* m_lightTransform;
	/*double m_xStart;
	double m_yStart;*/
public:
	CameraRotateState(StateMachine* stateMachine, Camera* camera) :
		State{ stateMachine, false }, m_camera{ camera }/*, 
		m_xStart{ stateMachine->mousePosX }, m_yStart{ stateMachine->mousePosY }*/,
		m_lightTransform{ m_stateMachine->m_lightSource->getComponent<Transform>() }
	{
		m_lightTransform->setRotation(m_camera->GetCameraRotation());
		//std::cout << "ENTERED CAMERA ROTATED\n";
	}
	virtual void enter() override;
	virtual void exit() override;
	virtual void onMouseMove(double x, double y) override;

};