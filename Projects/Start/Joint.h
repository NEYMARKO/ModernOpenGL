#pragma once

#include "Mesh.h"

class Joint
{
	private:
		Mesh* meshContainer;
		Joint* parent = nullptr;
		glm::quat orientation;
		glm::vec3 position;
		glm::vec3 tempPosition;
		glm::vec3 forward;
		glm::vec3 up;
		float length;
		float angleConstraint;

	public:

		Joint(float angleConstraint, Mesh* meshContainer);

		//Rotates joint's forward vector to face target
		void RotateTowardsTarget(glm::vec3& targetPos);

		//Returns false if joint has rotated past it's constraint (angle is too big)
		bool CanRotate();

		void Render();

		void SetParent(Joint* parent);
		void SetPosition(const glm::vec3& position);
		void SetTempPosition(const glm::vec3& tempPosition);
		glm::vec3 GetPosition();
		glm::vec3 GetForwardVector();
		glm::vec3 GetUpVector();
		float GetSegmentLength();

		Joint* GetParent();
		~Joint();
};