#pragma once

#include <vector>
#include <memory>

class Joint;
class Transform;
class MeshRenderer;

class KinematicChain
{
	private:
		
		std::vector<std::unique_ptr<Joint>> m_chain;
		std::vector<Transform*> m_jointsTransforms;
		std::unique_ptr<MeshRenderer> m_jointsMeshRenderer;

		glm::vec3 m_chainOrigin;
		Transform* m_targetTransform;
		float m_hardcodedLength = 1.0f;

		static int inline mIDGenerator{};
		int m_id{ mIDGenerator++ };

	public:

		KinematicChain(int numberOfJoints, float angleConstraint, 
			const glm::vec3& chainStartPos/*, Mesh* meshContainer, Mesh* target*/,
			Transform* targetTransform);
		
		void setMeshRenderer(std::unique_ptr<MeshRenderer> meshRenderer);


		void BackwardsPass();
		void ForwardPass();
		void FabrikAlgorithm(const int numberOfIterations);
		//runs FABRIK algorithm through "steps" iterations to determine positions of
		//joints in the chain. After that, it rotates each joint towards its target
		void simulate(const int steps);

		void moveTarget(float elapsedTime);

		//Returns true if distance between end of IK chain and target is smaller than defined ErrorMargin. Otherwise returns false
		bool ErrorTooSmall();
		bool targetOutOfReach();
		glm::vec3 getTargetPos();
		std::vector<std::unique_ptr<Joint>>* GetAllJoints() { return &m_chain; };
		std::vector<Transform*>& getJointsTransforms();
		MeshRenderer* getMeshRenderer();
		~KinematicChain();
};