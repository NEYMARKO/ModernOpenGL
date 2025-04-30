#include "Joint.h"
#define ERROR_MARGIN 0.5f
#define DOT_PRODUCT_ALLOWED_ERROR 0.01f
Joint::Joint(int id, float angleConstraint, Mesh* meshContainer) :
	mID{ id }, mAngleConstraint{ angleConstraint }, 
	mMeshContainer { meshContainer }, 
	mParent{ nullptr }, mChild{ nullptr },
	mLength{ std::max(
		abs(mMeshContainer->boundingBox->GetMaxExtremes().z - mMeshContainer->boundingBox->GetMinExtremes().z),
		abs(mMeshContainer->boundingBox->GetMaxExtremes().x - mMeshContainer->boundingBox->GetMinExtremes().x)
	) }, mForward{ glm::vec3(-1.0f, 0.0f, 0.0f) }, mOrientation{ glm::quat(glm::radians(0.0f), mForward) }

{
}

void Joint::RotateTowardsTarget(const glm::vec3& targetPos)
{
	/*glm::vec3 directionToTarget = glm::normalize(targetPos - mPosition);
	glm::quat rotationQuaternion = glm::rotation(mForward, directionToTarget);
	mOrientation = glm::normalize(rotationQuaternion * mOrientation);
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
	float parentToChildAngle = glm::acos(glm::dot(mForward, mParent->GetForwardVector()));

	return abs(parentToChildAngle) < mAngleConstraint ? true : false;
}

Joint::~Joint()
{
	std::cout << "Joint deleted " << std::endl;
}

void Joint::SetParent(Joint* parent)
{
	mParent = parent;
}

void Joint::SetChild(Joint* child)
{
	mChild = child;
}

void Joint::SetTempPosition(const glm::vec3& tempPosition)
{
	mTempPosition = tempPosition;
}

void Joint::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mJointEnd = mPosition + (mForward * mLength);
}