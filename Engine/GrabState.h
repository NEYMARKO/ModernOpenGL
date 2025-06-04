#pragma once
#include "TransformState.h"

#include <iostream>
class GrabState : public TransformState
{
public:
	GrabState(StateMachine* stateMachine, Camera* camera, Transform* objectTransform) :
		TransformState{ stateMachine, camera, objectTransform }
	{
		std::cout << "IN GRAB\n";
	}

	virtual void onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld) override;
	glm::vec3 projectPointToVector(const glm::vec3& point, const glm::vec3& vector);
};