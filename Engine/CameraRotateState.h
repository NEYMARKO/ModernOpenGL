#pragma once
#include "StateMachine.h"
#include "Camera.h"
#include "State.h"

class CameraRotateState : public State
{
private:
	Camera* m_camera;
	/*double m_xStart;
	double m_yStart;*/
public:
	CameraRotateState(StateMachine* stateMachine, Camera* camera) :
		State{ stateMachine, false }, m_camera{ camera }/*, 
		m_xStart{ stateMachine->mousePosX }, m_yStart{ stateMachine->mousePosY }*/
	{
		std::cout << "ENTERED CAMERA ROTATED\n";
	}
	virtual void onMouseMove(double x, double y) override;

};