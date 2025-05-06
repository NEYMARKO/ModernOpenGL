#include "Transform.h"
#include "Joint.h"

#include "glm/gtx/string_cast.hpp"
#include <iostream>

#define ERROR_MARGIN 0.5f
#define DOT_PRODUCT_ALLOWED_ERROR 0.01f
Joint::Joint(int id, float angleConstraint, float length/*, Mesh* meshContainer*/) :
	m_id{ id }, mAngleConstraint{ angleConstraint }/*, 
	mMeshContainer { meshContainer }*/, 
	m_parent{ nullptr }, m_child{ nullptr },
	m_length{ length }/*, mForward{ glm::vec3(-1.0f, 0.0f, 0.0f) }, mOrientation{ glm::quat(glm::radians(0.0f), mForward) }*/,
	m_transform{ std::make_unique<Transform>() }

{
	//joint's length is scaled to 1.0f
	//in meshloader it is visible that joint has xmin-xmax = 1.0f
	//since x is his foward vector and needs to be 1.0f, model is uniformly scaled down by it's length
	m_transform.get()->setScale(1 / 9.09666);
	//m_transform.get()->setRotation(glm::quat(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Joint::RotateTowardsTarget(const glm::vec3& targetPos)
{
	glm::vec3 directionToTarget = glm::normalize(targetPos - m_transform->getPosition());
	//glm::quat rotationQuaternion = glm::rotation(m_transform->getForwardVector(), directionToTarget);
	//if (glm::length(directionToTarget) < 0.1f) return;
	//std::cout << "DIRECTION TO TARGET: " << glm::to_string(directionToTarget) << std::endl;
	glm::quat rotationQuaternion = glm::rotation(-m_transform->getRightVector(), directionToTarget);
	//std::cout << "ROTATIOn quaternion: " << glm::to_string(rotationQuaternion) << std::endl;
	m_transform->rotate(rotationQuaternion);
	glm::vec3 forward = m_transform->getForwardVector();
	glm::vec3 trueForward = m_transform->getQuaternionRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
	/*mOrientation = glm::normalize(rotationQuaternion * mOrientation);
	mForward = directionToTarget;
	mMeshContainer->Rotate(mOrientation);*/
	
	
	//if (this->parent)
	//{
	//	float currentAngle = glm::acos(glm::clamp(glm::dot(this->forward, this->parent->forward), -1.0f, 1.0f));
	//	std::cout << "CURRENT ANGLE (id: " << id << ") : " << currentAngle << std::endl;
	//	glm::quat desiredRotation = glm::rotation(this->forward, directionToTarget);
	//	glm::vec3 desiredForward = glm::normalize(glm::rotate(desiredRotation, this->forward));
	//	float desiredAngle = glm::acos(glm::clamp(glm::dot(desiredForward, this->parent->forward), -1.0f, 1.0f));
	//	if (abs(desiredAngle) > glm::radians(this->angleConstraint))
	//	{
	//		float allowedRotation = glm::radians(this->angleConstraint) - abs(currentAngle);
	//		glm::vec3 rotationAxis = glm::normalize(glm::cross(this->forward, directionToTarget));

	//		glm::vec3 allowedForward = glm::rotate(this->forward, allowedRotation, rotationAxis);
	//		// Create a new clamped rotation quaternion
	//		glm::quat clampedRotation = glm::rotation(this->forward, allowedForward);

	//		// Apply the clamped rotation instead of the full rotation
	//		this->orientation = glm::normalize(clampedRotation * this->orientation);
	//		this->forward = glm::normalize(glm::rotate(clampedRotation, this->forward));
	//	}
	//	else 
	//	{
	//		// No constraint violation, proceed with the desired rotation
	//		this->orientation = glm::normalize(desiredRotation * this->orientation);
	//		this->forward = desiredForward;
	//	}
	//}
	//else
	//{
	//	glm::quat rotationQuaternion = glm::rotation(this->forward, directionToTarget);
	//	this->orientation = glm::normalize(rotationQuaternion * this->orientation);
	//	this->forward = directionToTarget;
	//}
	//this->meshContainer->Rotate(this->orientation);
}

bool Joint::CanRotate()
{
	float parentToChildAngle = glm::acos(glm::dot(m_transform->getForwardVector(), 
		m_parent->getForwardVector()));

	return abs(parentToChildAngle) < mAngleConstraint ? true : false;
}

Joint::~Joint()
{
	std::cout << "Joint deleted " << std::endl;
}

void Joint::SetParent(Joint* parent)
{
	m_parent = parent;
}

void Joint::SetChild(Joint* child)
{
	m_child = child;
}

void Joint::SetTempPosition(const glm::vec3& tempPosition)
{
	m_tempPosition = tempPosition;
}

void Joint::SetPosition(const glm::vec3& position)
{
	m_transform.get()->setPosition(position);
	//mJointEnd = mPosition + (mForward * m_length);
}

glm::vec3 Joint::getPosition() 
{ 
	return m_transform->getPosition(); 
};

glm::vec3 Joint::getTempPosition() 
{ 
	return m_tempPosition; 
};

glm::vec3 Joint::getForwardVector() 
{ 
	return -m_transform.get()->getRightVector();
};

glm::vec3 Joint::getJointEnd() 
{
	return m_transform.get()->getPosition() -
		m_transform.get()->getRightVector() * m_length;
	/*return m_transform.get()->getPosition() +
		m_transform.get()->getForwardVector() * m_length;*/
};

Transform* Joint::getTransform()
{
	return m_transform.get();
}