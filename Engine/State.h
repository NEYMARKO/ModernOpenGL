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
	CAMERA_MOVE,
	CAMERA_ROTATE,
	ADD,
	DELETE
};

struct Hit
{
	Object* obj;
	glm::vec3 point;
};

class State
{
protected:
	States m_transitionState = States::NO_TRANSITION;
	StateMachine* m_stateMachine;
public:
	State(StateMachine* stateMachine, bool convertTo3D = false) :
		m_stateMachine{ stateMachine }, m_convertMouseTo3D { convertTo3D }
	{
	}
	bool m_convertMouseTo3D{ false };
	/*bool m_movingCamera{ false };*/
	virtual void enter() {};
	virtual void exit() {};
	virtual void update() {};
	virtual void onMouseMove(double x, double y) {};
	virtual void onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld) {};
	virtual void onMouseClick(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld,
		int button, int action);
	virtual void onKeyboardPress(int key, int action);
	//Sorts objects using 2 conditions: by layer (starting from those that have higher priority layer),
	//and by distance (those closer to ray start have advantage)
	void sortObjects(std::vector<Hit>& hits, const glm::vec3& start);
	void updateSelection(const std::vector<Hit>& hits);
	States getTransitionState() { return m_transitionState; }
	virtual ~State() {};
};