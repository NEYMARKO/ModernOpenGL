#pragma once

enum States
{
	EMPTY,
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
	enum States transitionState = States::EMPTY;
public:
	virtual void enter();
	virtual void exit();
	void onMouseClick();
	void onMouseMove();
	void onKeyboardPress();
	States getTransitionState() { return transitionState; }
	virtual ~State();
};