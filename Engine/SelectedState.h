#pragma once
#include "State.h"
#include "Object.h"

class SelectedState : public State
{
private:
	Object* m_selection;
public:
	SelectedState(StateMachine* stateMachine, Object* selectedObject) :
		State { stateMachine }, m_selection { selectedObject }
	{ }
	virtual void enter() {};
	void exit() {}
	void onMouseMove() {};
	void onMouseClick() {};
	virtual void onKeyboardPress(int key) override;
};