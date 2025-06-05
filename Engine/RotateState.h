#pragma once
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include "Transform.h"
#include "TransformState.h"

class RotateState : public TransformState
{
private:
	float m_radius{15.0f};
	glm::vec3 m_oldIntersection{ 0.0f, 0.0f, 0.0f };
	glm::quat m_originalRotation;
public:
	RotateState(StateMachine* stateMachine, Camera* camera, Transform* objectTransform) :
		TransformState{ stateMachine, camera, objectTransform },
		m_originalRotation{ objectTransform->getQuaternionRotation() }
	{
		std::cout << "IN ROTATE\n";
	}
	virtual void onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld) override;
	glm::vec3 projectPointToCircle(const glm::vec3& point);
};