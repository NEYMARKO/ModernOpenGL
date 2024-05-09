#include "Joint.h"
#define ERROR_MARGIN 0.1f
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

	if (!this->child && glm::distance(this->position + (this->forward * this->length), targetPos) <= ERROR_MARGIN)
	{
		return;
	}
	//glm::vec3 directionToTarget = glm::normalize(targetPos - (this->position + (this->forward * this->length)));
	glm::vec3 directionToTarget = glm::normalize(targetPos - this->position);

	//float rotationAngle = glm::acos(glm::clamp(glm::dot(this->forward, directionToTarget), -1.0f, 1.0f));
	
	/*float forwardLen = glm::length(this->forward);
	float directionLen = glm::length(directionToTarget);*/

	float dotResult = glm::dot(this->forward, directionToTarget);
	dotResult = glm::clamp(dotResult, -1.0f, 1.0f);
	float rotationAngle = glm::acos(dotResult);

	//vectors are parallel, no need to update rotation
	if (abs(dotResult) == 1) return;
	//std::cout << "DOT RESULT: " << dotResult << std::endl;
	//if (abs(dotResult) >= 1)
	//{
	//	//std::cout << "ENTERED IF " << std::endl;
	//	rotationAngle = glm::radians(0.0f);
	//}

	//std::cout << "ROTATION ANGLE: " << rotationAngle << std::endl;
	/*if (isnan(rotationAngle))
	{

		std::cout << "DOT RESULT: " << dotResult << std::endl;
		std::cout << "ROTATION ANGLE: " << rotationAngle << std::endl;
		
		std::cout << "FORWARD: ";
		PrintClass::PrintVec3(this->forward);

		std::cout << "FORWARD LEN: " << forwardLen << std::endl;

		std::cout << "DIRECTION: ";
		PrintClass::PrintVec3(directionToTarget);

		std::cout << "DIRECTION LEN: " << directionLen << std::endl<<std::endl;

	}*/

	//angle between parent's forward vector and child's future forward direction is larger than constraint allows
	/*if (!this->parent && abs(rotationAngle) > glm::radians(this->angleConstraint))
	{
		return;
	}*/

	glm::vec3 rotationAxis = glm::cross(this->forward, directionToTarget);

	/*if (glm::length(rotationAxis) < 0.001f)
	{
		return;
	}*/
	//if (glm::length(rotationAxis) < 0.001f) { // Threshold for near-zero vectors
	//	rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);  // Default axis
	//}
	glm::quat rotationQuaternion = glm::normalize(glm::angleAxis(rotationAngle, rotationAxis));

	/*std::cout << "Rotation quat: ";
	PrintClass::PrintQuat(rotationQuaternion);*/


	/*std::cout << "Orientation quat: ";
	PrintClass::PrintQuat(this->orientation);*/

	rotationQuaternion = glm::rotation(this->forward, directionToTarget);
	this->orientation = glm::normalize(rotationQuaternion * this->orientation);


	this->forward = glm::rotate(rotationQuaternion, this->forward);
	//this->forward = directionToTarget;
	this->meshContainer->Rotate(this->orientation);
	
	
	
	
	//if (!this->child)
	//{
	//	glm::vec3 directionToTarget = glm::normalize(targetPos - this->jointEnd);
	//	float rotationAngle = glm::acos(glm::radians(glm::dot(this->forward, directionToTarget)));

	//	//angle between parent's forward vector and child's future forward direction is larger than constraint allows
	//	if (this->parent && abs(rotationAngle) > this->angleConstraint)
	//	{
	//		return;
	//	}

	//	glm::vec3 rotationAxis = glm::cross(this->forward, directionToTarget);
	//	glm::quat rotationQuaternion = glm::normalize(glm::angleAxis(rotationAngle, rotationAxis));

	//	/*std::cout << "Rotation quat: ";
	//	PrintClass::PrintQuat(rotationQuaternion);*/

	//	this->orientation = glm::normalize(rotationQuaternion * this->orientation);

	//	/*std::cout << "Orientation quat: ";
	//	PrintClass::PrintQuat(this->orientation);*/

	//	this->forward = directionToTarget;
	//	this->meshContainer->Rotate(this->orientation);
	//}
	//else
	//{
	//	glm::vec3 directionToTarget = glm::normalize(this->GetChild()->GetPosition() - this->jointEnd);

	//	if (this->parent && abs(glm::acos(glm::radians(glm::dot(this->parent->forward, directionToTarget)))) > this->angleConstraint)
	//	{
	//		return;
	//	}
	//	float rotationAngle = glm::acos(glm::radians(glm::dot(this->forward, directionToTarget)));
	//	glm::vec3 rotationAxis = glm::cross(this->forward, directionToTarget);
	//	glm::quat rotationQuaternion = glm::normalize(glm::angleAxis(rotationAngle, rotationAxis));

	//	/*std::cout << "Rotation quat: ";
	//	PrintClass::PrintQuat(rotationQuaternion);*/

	//	this->orientation = glm::normalize(rotationQuaternion * this->orientation);

	//	/*std::cout << "Orientation quat: ";
	//	PrintClass::PrintQuat(this->orientation);*/

	//	this->forward = directionToTarget;
	//	this->meshContainer->Rotate(this->orientation);
	//}
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