#pragma once

#include <vector>
#include <memory>

class Joint;
class Mesh;
class Gizmos;

class KinematicChain
{
	private:
		
		glm::vec3 mChainOrigin;
		std::vector<std::unique_ptr<Joint>> mChain;
		Mesh* mTarget;

		static int inline mIDGenerator{};
		int mID{ mIDGenerator++ };

	public:

		KinematicChain(int numberOfJoints, float angleConstraint, 
			const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target, Gizmos* gizmos);

		void BackwardsPass();
		void ForwardPass();
		void FabrikAlgorithm(const int numberOfIterations);

		glm::vec3 CalculateNewJointPosition(Joint* joint, const float direction);

		//Returns true if distance between end of IK chain and target is smaller than defined ErrorMargin
		bool ErrorTooSmall();
		bool targetOutOfReach();
		Mesh* GetTarget() { return mTarget; };
		std::vector<std::unique_ptr<Joint>>* GetAllJoints() { return &mChain; };

		~KinematicChain();
};