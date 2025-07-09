#include "Transform.h"
#include "Joint.h"

#define ERROR_MARGIN 0.5f
#define DOT_PRODUCT_ALLOWED_ERROR 0.01f
Joint::Joint(int id, float angleConstraint, float length) :
	m_id{ id }, mAngleConstraint{ angleConstraint },
	m_parent{ nullptr }, m_child{ nullptr },
	m_length{ length },
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
	glm::quat rotationQuaternion = glm::rotation(-m_transform->getRightVector(), directionToTarget);
	m_transform->rotate(rotationQuaternion);
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
	//mJointEnd = m_position + (mForward * m_length);
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