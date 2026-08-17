#pragma once
#include "./State.h"
#include "../../SceneEntity.h"

#include <iostream>
class SelectedState : public State
{
private:
	SceneEntity* m_selection;
public:
	SelectedState(StateMachine* stateMachine, SceneEntity* selectedObject) :
		State { stateMachine }, m_selection { selectedObject }
	{
		std::cout << "IN SELECTED\n";
	}
	virtual void exit() { std::cout << "EXITED SELECTED, TARGET SHOULD BE SAME\n"; };
	virtual void onKeyboardPress(int key, int action) override;
};