#pragma once

#include "Joint.h"

class KinematicChain
{
	private:
		std::vector<Joint*> chain;
		Mesh* target;
	public:
		KinematicChain(int numberOfJoints, float angleConstraint, Mesh* meshContainer, Mesh* target);

		void FabrikAlgorithm(const int numberOfIterations);

		void ForwardPass();

		void BackwardsPass();

		glm::vec3 CalculateNewJointPosition(Joint* joint, const float direction);

		glm::vec3 NewBackwardJointPosition(Joint* joint);

		~KinematicChain();
};