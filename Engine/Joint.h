#pragma once

#include "Gizmos.h"
#include <algorithm>
class Joint
{
	private:
		Mesh* meshContainer;
		Joint* parent = nullptr;
		Joint* child = nullptr;
		glm::quat orientation;
		glm::vec3 position;
		glm::vec3 jointEnd;
		glm::vec3 tempPosition;
		glm::vec3 forward;
		glm::vec3 up;
		float length;
		float angleConstraint;
		int id;

	public:

		Joint(float angleConstraint, Mesh* meshContainer, int id);

		//Rotates joint's forward vector to face the target
		void RotateTowardsTarget(const glm::vec3& targetPos);

		//Returns false if joint has rotated past it's constraint (angle is too big)
		bool CanRotate();

		void Render();

		void SetParent(Joint* parent);
		void SetChild(Joint* child);
		void SetPosition(const glm::vec3& position);
		void SetTempPosition(const glm::vec3& tempPosition);

		int GetID();
		glm::vec3 GetPosition();
		glm::vec3 GetTempPosition();
		glm::vec3 GetForwardVector();
		glm::vec3 GetJointEnd();
		float GetSegmentLength();
		Mesh* GetMeshContainer();
		Joint* GetParent();
		Joint* GetChild();
		~Joint();
};