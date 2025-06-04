#pragma once
#include "State.h"
#include "Object.h"

#include <iostream>
class SelectedState : public State
{
private:
	Object* m_selection;
public:
	SelectedState(StateMachine* stateMachine, Object* selectedObject) :
		State { stateMachine }, m_selection { selectedObject }
	{
		std::cout << "IN SELECTED\n";
	}
	virtual void enter() {};
	void exit() {}
	void onMouseMove() {};
	void onMouseClick() {};
	virtual void onKeyboardPress(int key, int action) override;
};