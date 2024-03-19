#pragma once

#include "Joint.h"

class KinematicChain
{
	private:
		Mesh* target;
		Mesh* meshContainer;
		std::vector<Joint*> chain;
		glm::vec3 chainStartPos;
	public:
		KinematicChain(int numberOfJoints, float angleConstraint, const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target);

		void FabrikAlgorithm(const int numberOfIterations);

		void ForwardPass();

		void BackwardsPass();

		glm::vec3 CalculateNewJointPosition(Joint* joint, const float direction);

		bool ErrorTooSmall();

		std::vector<Joint*>* GetAllJoints();

		~KinematicChain();
};