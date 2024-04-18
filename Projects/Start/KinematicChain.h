#pragma once

#include "Joint.h"

class KinematicChain
{
	private:
		Mesh* target;
		std::vector<Joint*> chain;
		glm::vec3 chainStartPos;
	public:
		KinematicChain(int numberOfJoints, float angleConstraint, const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target);

		void FabrikAlgorithm(const int numberOfIterations);

		void ForwardPass();

		void BackwardsPass();

		glm::vec3 CalculateNewJointPosition(Joint* joint, const float direction);

		//Returns true if distance between end of IK chain and target is smaller than defined ErrorMargin
		bool ErrorTooSmall();

		std::vector<Joint*>* GetAllJoints();
		Mesh* GetTarget();

		~KinematicChain();
};