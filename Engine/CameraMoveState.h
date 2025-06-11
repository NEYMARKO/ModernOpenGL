#pragma once
#include "StateMachine.h"
#include "Camera.h"
#include "State.h"

#define MIN_LENGTH 0.01f

class CameraMoveState : public State
{
private:
	Camera* m_camera;
	glm::vec3 m_moveDir{};
	glm::vec3 m_zeroVector{MIN_LENGTH};
	//int m_key;
public:
	CameraMoveState(StateMachine* stateMachine, Camera* camera) :
		State{ stateMachine }, m_camera{ camera }/*, 
		m_key { stateMachine->m_lastKey }*/
	{
		std::cout << "IN CAMERA MOVE\n";
		//m_movingCamera = true;
		updateMoveDirection(m_stateMachine->m_lastKey, GLFW_PRESS);
	}
	virtual void update() override;
	virtual void onKeyboardPress(int key, int action) override;
	void updateMoveDirection(int key, int action);
};