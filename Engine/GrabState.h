#pragma once
#include "TransformState.h"

class GrabState : public TransformState
{
public:
	void move();
	void onMouseMove(const double mouseX, const double mouseY);
};