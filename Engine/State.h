#pragma once
#include <glm/glm.hpp>
#include <vector>

class GLFWwindow;
class Object;
class StateMachine;

enum States
{
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
	States m_transitionState = States::DEFAULT;
	StateMachine* m_stateMachine;
public:
	State(StateMachine* stateMachine) :
		m_stateMachine{ stateMachine }
	{
	}
	virtual void enter() {};
	virtual void exit() {};
	void onMouseClick(GLFWwindow* window, int button, int action);
	//Sorts objects using 2 conditions: by layer (starting from those that have higher priority layer),
	//and by distance (those closer to ray start have advantage)
	void sortObjects(std::vector<Object*>& objects, const glm::vec3& start);
	void updateSelection(const std::vector<Object*>& objects);
	void onMouseMove() {};
	void onKeyboardPress() {};
	States getTransitionState() { return m_transitionState; }
	virtual ~State() {};
};