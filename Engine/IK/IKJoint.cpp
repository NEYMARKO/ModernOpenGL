#include "../Components/Transform.h"
#include "./IKJoint.h"

#define ERROR_MARGIN 0.5f
#define DOT_PRODUCT_ALLOWED_ERROR 0.01f
IKJoint::IKJoint(int id, float angleConstraint, float length) :
	m_id{ id }, mAngleConstraint{ angleConstraint },
	m_parent{ nullptr }, m_child{ nullptr },
	m_length{ length },
	m_transform{ std::make_unique<Transform>() }

{}

void IKJoint::RotateTowardsTarget(const glm::vec3& targetPos)
{
	glm::vec3 directionToTarget = glm::normalize(targetPos - m_transform->getPosition());
	glm::quat rotationQuaternion = glm::rotation(-m_transform->getRightVector(), directionToTarget);
	m_transform->rotate(rotationQuaternion);
}

bool IKJoint::CanRotate()
{
	float parentToChildAngle = glm::acos(glm::dot(m_transform->getForwardVector(), 
		m_parent->getForwardVector()));

	return abs(parentToChildAngle) < mAngleConstraint ? true : false;
}

IKJoint::~IKJoint()
{
	std::cout << "Joint deleted " << std::endl;
}

void IKJoint::SetParent(IKJoint* parent)
{
	m_parent = parent;
}

void IKJoint::SetChild(IKJoint* child)
{
	m_child = child;
}

void IKJoint::SetTempPosition(const glm::vec3& tempPosition)
{
	m_tempPosition = tempPosition;
}

void IKJoint::SetPosition(const glm::vec3& position)
{
	m_transform.get()->setPosition(position);
	//mJointEnd = m_position + (mForward * m_length);
}

glm::vec3 IKJoint::getPosition() 
{ 
	return m_transform->getPosition(); 
};

glm::vec3 IKJoint::getTempPosition() 
{ 
	return m_tempPosition; 
};

glm::vec3 IKJoint::getForwardVector() 
{ 
	return -m_transform.get()->getRightVector();
};

glm::vec3 IKJoint::getJointEnd() 
{
	return m_transform.get()->getPosition() -
		m_transform.get()->getRightVector() * m_length;
	/*return m_transform.get()->getPosition() +
		m_transform.get()->getForwardVector() * m_length;*/
};

Transform* IKJoint::getTransform()
{
	return m_transform.get();
}