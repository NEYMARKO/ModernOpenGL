#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>

class Transform;
//#include "Transform.h"

class Joint
{
	private:

		std::unique_ptr<Transform> m_transform;
		glm::vec3 m_tempPosition;
		//Transform* m_tempTransform;
		//Transform* m_parentTransform;
		//Transform* m_childTransform;
		int m_id;
		float mAngleConstraint;
		Joint* m_parent;
		Joint* m_child;
		//glm::vec3 mForward;
		//glm::quat mOrientation;
		//glm::vec3 m_position;
		//glm::vec3 mJointEnd;
		//glm::vec3 mTempPosition;
		//glm::vec3 mUp;
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
		/*Mesh* GetMeshContainer() { return mMeshContainer; };*/
		~Joint();
};