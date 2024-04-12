#include "Joint.h"

Joint::Joint(float angleConstraint, Mesh* meshContainer)
{
	this->angleConstraint = angleConstraint;
	this->meshContainer = meshContainer;
	this->length = std::max(
		abs(this->meshContainer->boundingBox->GetMaxExtremes().z - this->meshContainer->boundingBox->GetMinExtremes().z),
		abs(this->meshContainer->boundingBox->GetMaxExtremes().x - this->meshContainer->boundingBox->GetMinExtremes().x)
	);
	this->forward = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void Joint::RotateTowardsTarget(glm::vec3& targetPos)
{
	glm::vec3 directionToTarget = glm::normalize(targetPos - this->position);
	float rotationAngle = glm::acos(glm::dot(this->forward, directionToTarget));
	
	//angle between parent's forward vector and child's future forward direction is larger than constraint allows
	if (abs(glm::dot(this->parent->forward, directionToTarget)) > this->angleConstraint)
	{
		return;
	}

	glm::vec3 rotationAxis = glm::cross(this->forward, directionToTarget);
	glm::quat rotationQuaternion = glm::angleAxis(rotationAngle, rotationAxis);
	
	this->orientation = rotationQuaternion * this->orientation;
	this->forward = directionToTarget;
	this->meshContainer->Rotate(orientation);

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