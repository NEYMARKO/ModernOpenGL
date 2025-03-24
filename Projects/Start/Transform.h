#pragma once
#include "Mesh.h"


enum TransformState 
{
	GRAB,
	SCALE,
	ROTATE
};

class Transform
{
	private:
		Camera* camera;
		Mesh* target;
		TransformState activeState;

		//Sets the initial state
		Transform();

		TransformState GetActiveState();
		void SetActiveState(TransformState& state);
};