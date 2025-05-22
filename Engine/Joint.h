#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>

class Transform;

class Joint
{
	private:

		std::unique_ptr<Transform> m_transform;
		glm::vec3 m_tempPosition;
		int m_id;
		float mAngleConstraint;
		Joint* m_parent;
		Joint* m_child;
		float m_length;

	public:

		Joint(int id, float angleConstraint, float length/*, Mesh* meshContainer*/);

		//Rotates joint's forward vector to face the target
		void RotateTowardsTarget(const glm::vec3& targetPos);

		//Returns false if joint has rotated past it's constraint (angle is too big)
		bool CanRotate();

		/*void Render();*/

		void SetParent(Joint* parent);
		void SetChild(Joint* child);
		void SetPosition(const glm::vec3& position);
		void SetTempPosition(const glm::vec3& tempPosition);

		Joint* GetChild() { return m_child; };
		Joint* GetParent() { return m_parent; };
		float GetSegmentLength() { return m_length; };
		int GetID() { return m_id; };
		glm::vec3 getPosition();
		glm::vec3 getTempPosition();
		glm::vec3 getForwardVector();
		glm::vec3 getJointEnd();
		Transform* getTransform();
		~Joint();
};