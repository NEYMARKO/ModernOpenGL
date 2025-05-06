#pragma once

#include <vector>
#include <memory>

class Joint;
class Transform;
class MeshRenderer;
//class Gizmos;

class KinematicChain
{
	private:
		
		std::vector<std::unique_ptr<Joint>> m_chain;
		std::vector<Transform*> m_jointsTransforms;

		glm::vec3 m_chainOrigin;
		Transform* m_targetTransform;
		std::unique_ptr<MeshRenderer> m_meshRenderer;
		float m_hardcodedLength = 1.0f;

		static int inline mIDGenerator{};
		int m_id{ mIDGenerator++ };

	public:

		KinematicChain(int numberOfJoints, float angleConstraint, 
			const glm::vec3& chainStartPos/*, Mesh* meshContainer, Mesh* target*/,
			Transform* targetTransform/*,
			Gizmos* gizmos*/);
		
		void setMeshRenderer(std::unique_ptr<MeshRenderer> meshRenderer);

		void simulate(const int steps);

		void BackwardsPass();
		void ForwardPass();
		void FabrikAlgorithm(const int numberOfIterations);

		void moveTarget(float elapsedTime);

		glm::vec3 CalculateNewJointPosition(Joint* joint, const float direction);

		//Returns true if distance between end of IK chain and target is smaller than defined ErrorMargin
		bool ErrorTooSmall();
		bool targetOutOfReach();
		glm::vec3 getTargetPos();
		std::vector<std::unique_ptr<Joint>>* GetAllJoints() { return &m_chain; };
		std::vector<Transform*>& getJointsTransforms();
		MeshRenderer* getMeshRenderer();
		//void simulate();
		~KinematicChain();
};