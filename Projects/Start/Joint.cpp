#include "Joint.h"

Joint::Joint(float angleConstraint, Mesh* meshContainer)
{
	this->angleConstraint = angleConstraint;
	this->meshContainer = meshContainer;
	this->length = abs(this->meshContainer->boundingBox->GetMaxExtremes().x - this->meshContainer->boundingBox->GetMinExtremes().x);
	/*this->length = std::max(
		abs(this->meshContainer->boundingBox->GetMaxExtremes().z - this->meshContainer->boundingBox->GetMinExtremes().z),
		abs(this->meshContainer->boundingBox->GetMaxExtremes().x - this->meshContainer->boundingBox->GetMinExtremes().x)
	);*/
	/*this->length = std::max(this->length, 
		abs(this->meshContainer->boundingBox->GetMaxExtremes().y - this->meshContainer->boundingBox->GetMinExtremes().y));*/
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

void Joint::SetChild(Joint* child)
{
	this->child = child;
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

glm::vec3 Joint::GetTempPosition()
{
	return this->tempPosition;
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

Joint* Joint::GetChild()
{
	return this->child;
}

float Joint::GetSegmentLength()
{
	return this->length;
}

Mesh* Joint::GetMeshContainer()
{
	return this->meshContainer;
}