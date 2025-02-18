#include "Joint.h"
#define ERROR_MARGIN 0.5f
#define DOT_PRODUCT_ALLOWED_ERROR 0.01f
Joint::Joint(float angleConstraint, Mesh* meshContainer, int id)
{
	this->id = id;
	this->angleConstraint = angleConstraint;
	this->meshContainer = meshContainer;
	this->length = std::max(
		abs(this->meshContainer->boundingBox->GetMaxExtremes().z - this->meshContainer->boundingBox->GetMinExtremes().z),
		abs(this->meshContainer->boundingBox->GetMaxExtremes().x - this->meshContainer->boundingBox->GetMinExtremes().x)
	);
	//this->jointEnd = this->position + this->forward * this->length;
	this->forward = glm::vec3(-1.0f, 0.0f, 0.0f);
	this->orientation = glm::quat(glm::radians(0.0f), this->forward);
}

void Joint::RotateTowardsTarget(const glm::vec3& targetPos)
{
	glm::vec3 directionToTarget = glm::normalize(targetPos - this->position);
	glm::quat rotationQuaternion = glm::rotation(this->forward, directionToTarget);
	this->orientation = glm::normalize(rotationQuaternion * this->orientation);
	this->forward = directionToTarget;
	this->meshContainer->Rotate(this->orientation);
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
	this->jointEnd = this->position + (this->forward * this->length);
}

int Joint::GetID()
{
	return this->id;
}
glm::vec3 Joint::GetPosition()
{
	return this->position;
}

glm::vec3 Joint::GetJointEnd()
{
	return this->jointEnd;
}
glm::vec3 Joint::GetTempPosition()
{
	return this->tempPosition;
}

glm::vec3 Joint::GetForwardVector()
{
	return this->forward;
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