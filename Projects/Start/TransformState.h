#pragma once
#include "StateMachine/Transform.h"

class Transform;

class TransformState
{
	public:
		virtual void Enter(Transform* transform);
		virtual void ChangeState(Transform* transform);
		virtual void Exit(Transfrom* transform);
		virtual ~TransformState();
};