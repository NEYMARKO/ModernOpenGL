#include "Joint.h"

Joint::Joint(float angleConstraint, Mesh* meshContainer)
{
	this->angleConstraint = angleConstraint;
	this->meshContainer = meshContainer;
	this->length = abs(this->meshContainer->boundingBox->GetMaxExtremes().z - this->meshContainer->boundingBox->GetMinExtremes().z);
}

void Joint::RotateTowardsTarget(glm::vec3& targetPos)
{
	//Angle between target and joint

	float rotationAngle = glm::acos(glm::dot(this->forward, glm::normalize(targetPos - this->position)));

	//Rotate forward and up vector using quaternion
}

bool Joint::CanRotate()
{
	float parentToChildAngle = glm::acos(glm::dot(this->forward, this->parent->GetForwardVector()));

	return abs(parentToChildAngle) < this->angleConstraint ? true : false;
}

Joint::~Joint()
{
	std::cout << "Joint deleted " << std::endl;
}

void Joint::SetParent(Joint* parent)
{
	this->parent = parent;
}

void Joint::SetTempPosition(const glm::vec3& tempPosition)
{
	this->tempPosition = tempPosition;
}

void Joint::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

glm::vec3 Joint::GetPosition()
{
	return this->position;
}

glm::vec3 Joint::GetForwardVector()
{
	return this->forward;
}

glm::vec3 Joint::GetUpVector()
{
	return this->up;
}

Joint* Joint::GetParent()
{
	return this->parent;
}

float Joint::GetSegmentLength()
{
	return this->length;
}

Mesh* Joint::GetMeshContainer()
{
	return this->meshContainer;
}