#pragma once

#include "Gizmos.h"
#include <algorithm>
class Joint
{
	private:
		int mID;
		float mAngleConstraint;
		Mesh* mMeshContainer;
		Joint* mParent;
		Joint* mChild;
		glm::vec3 mForward;
		glm::quat mOrientation;
		glm::vec3 mPosition;
		glm::vec3 mJointEnd;
		glm::vec3 mTempPosition;
		glm::vec3 mUp;
		float mLength;

	public:

		Joint(int id, float angleConstraint, Mesh* meshContainer);

		//Rotates joint's forward vector to face the target
		void RotateTowardsTarget(const glm::vec3& targetPos);

		//Returns false if joint has rotated past it's constraint (angle is too big)
		bool CanRotate();

		void Render();

		void SetParent(Joint* parent);
		void SetChild(Joint* child);
		void SetPosition(const glm::vec3& position);
		void SetTempPosition(const glm::vec3& tempPosition);

		int GetID() { return mID; };
		glm::vec3 GetPosition() { return mPosition; };
		glm::vec3 GetTempPosition() { return mTempPosition; };
		glm::vec3 GetForwardVector() { return mForward; };
		glm::vec3 GetJointEnd() { return mJointEnd; };
		float GetSegmentLength() { return mLength; };
		Mesh* GetMeshContainer() { return mMeshContainer; };
		Joint* GetParent() { return mParent; };
		Joint* GetChild() { return mChild; };
		~Joint();
};