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
	if (!this->child && glm::distance(this->position + (this->forward * this->length), targetPos) <= ERROR_MARGIN)
	{
		return;
	}
	//glm::vec3 directionToTarget = glm::normalize(targetPos - (this->position + (this->forward * this->length)));
	glm::vec3 directionToTarget = glm::normalize(targetPos - this->position);


	std::cout << "ORIENTATION (" << this->id << ") BEFORE STOPPING: ";
	PrintClass::PrintQuat(this->orientation);

	/*std::cout << "FORWARD: " << std::endl;
	PrintClass::PrintVec3(this->forward);
	std::cout << "DIRECTION: " << std::endl;
	PrintClass::PrintVec3(directionToTarget);*/
	float dotResult = glm::dot(this->forward, directionToTarget);
	dotResult = glm::clamp(dotResult, -1.0f, 1.0f);
	/*if (abs(dotResult - 1.0f) < DOT_PRODUCT_ERROR || abs(dotResult + 1.0f) < DOT_PRODUCT_ERROR)
	{*/
	//if (abs(dotResult) >= 1 - DOT_PRODUCT_ALLOWED_ERROR)
	//{
	//	/*std::cout << "JOINT " << this->id << " STOPPED MOVING" << std::endl;
	//	std::cout << "ORIENTATION (" << this->id << ") AFTER STOPPING: ";
	//	PrintClass::PrintQuat(this->orientation);
	//	std::cout << std::endl << std::endl;*/
	//	std::cout << "FORWARD: " << std::endl;
	//	PrintClass::PrintVec3(this->forward);
	//	std::cout << "DIRECTION: " << std::endl;
	//	PrintClass::PrintVec3(directionToTarget);

	//	//std::cout << "EQUAL?: " << (this->forward == directionToTarget) << std::endl;
	//	return;
	//}

	/*std::cout << "JOINT " << this->id << " PASSED" << std::endl;
	std::cout << "FORWARD: " << std::endl;
	PrintClass::PrintVec3(this->forward);
	std::cout << "DIRECTION: " << std::endl;
	PrintClass::PrintVec3(directionToTarget);*/
	//float rotationAngle = glm::acos(dotResult);
	//vectors are parallel, no need to update rotation

	/*glm::vec3 rotationAxis = glm::cross(this->forward, directionToTarget);
	glm::quat rotationQuaternion = glm::normalize(glm::angleAxis(rotationAngle, rotationAxis));*/

	glm::quat rotationQuaternion = glm::rotation(this->forward, directionToTarget);
	this->orientation = glm::normalize(rotationQuaternion * this->orientation);
	//if (this->forward == directionToTarget) PrintClass::PrintQuat(rotationQuaternion);


	//this->forward = glm::normalize(glm::rotate(rotationQuaternion, this->forward));
	this->forward = directionToTarget;
	this->meshContainer->Rotate(this->orientation);
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