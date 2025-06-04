#pragma once
#include <glm/glm.hpp>
#include <vector>
//#include "OpenGLIncludes.h"
class Object;
class StateMachine;

enum States
{
	NO_TRANSITION,
	DEFAULT,
	IDLE,
	SELECTED,
	GRAB,
	ROTATE,
	SCALE,
	ADD,
	DELETE
};

class State
{
protected:
	States m_transitionState = States::NO_TRANSITION;
	StateMachine* m_stateMachine;
public:
	State(StateMachine* stateMachine) :
		m_stateMachine{ stateMachine }
	{
	}
	virtual void enter() {};
	virtual void exit() {};
	virtual void onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld) {};
	virtual void onMouseClick(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld,
		int button, int action);
	virtual void onKeyboardPress(int key, int action) {};
	//Sorts objects using 2 conditions: by layer (starting from those that have higher priority layer),
	//and by distance (those closer to ray start have advantage)
	void sortObjects(std::vector<Object*>& objects, const glm::vec3& start);
	void updateSelection(const std::vector<Object*>& objects);
	States getTransitionState() { return m_transitionState; }
	virtual ~State() {};
};