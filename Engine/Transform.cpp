#include "Transform.h"
#include "Object.h"

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: mPosition{ position }, mRotation{ rotation }, mScale{ scale }
{
	//position, rotation and scale have changed, model matrix needs to be updated
	mDirty = true;
}

void Transform::translate(const glm::vec3& translation)
{
	mPosition += translation;
	mDirty = true;
}

void Transform::rotate(const glm::vec3& axis, float angle)
{
	glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
	mRotation = glm::normalize(rotation * mRotation);
	mDirty = true;
}

void Transform::rotate(const glm::quat& rotation)
{
	mRotation = glm::normalize(rotation * mRotation);
	mDirty = true;
}

void Transform::scale(const glm::vec3& scale)
{
	mScale += scale;
	mDirty = true;
}

void Transform::scale(float scale)
{
	mScale += glm::vec3(scale, scale, scale);
	mDirty = true;
}

void Transform::setPosition(const glm::vec3& position)
{
	mPosition = position;
	mDirty = true;
}

void Transform::setRotation(const glm::quat& rotation)
{
	mRotation = rotation;
	mDirty = true;
}

void Transform::setScale(const glm::vec3& scale)
{
	mScale = scale;
	mDirty = true;
}

void Transform::setScale(float scale)
{
	mScale = glm::vec3(scale, scale, scale);
	mDirty = true;
}

void Transform::updateModelMatrix()
{
	//Transformation order is happening in reverse of how it is written
	//read from bottom to top

	//m = I
	//m_t = glm::translate(m, pos) => m_t = m * T = I * T = T
	//m_r = glm::rotate(m_t, angle, axis) => m_r = m_t * R => I * T * R = T * R 
	//m_s = glm::scale(m_r, scale) => m_s = m_r * S => m * T * R * S = T * R * S
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::translate(mModelMatrix, mPosition);
	//mModelMatrix = glm::rotate(mModelMatrix, glm::radians(0.0f), glm::vec3(0.0, 0.0, 0.0));
	mModelMatrix *= glm::toMat4(mRotation);
	mModelMatrix = glm::scale(mModelMatrix, mScale);
	mDirty = false;
}

void Transform::setParent(Object* parent)
{
	mParentObject = parent;
}

glm::vec3 Transform::getRightVector()
{
	if (mDirty)
		updateModelMatrix();
	return glm::normalize(glm::vec3(mModelMatrix[0]));
}

glm::vec3 Transform::getUpVector()
{
	if (mDirty)
		updateModelMatrix();
	return glm::normalize(glm::vec3(mModelMatrix[1]));
}

glm::vec3 Transform::getForwardVector()
{
	if (mDirty)
		updateModelMatrix();
	return glm::normalize(glm::vec3(mModelMatrix[2]));
}

glm::mat4 Transform::getModelMatrix()
{
	if (mDirty)
		updateModelMatrix();
	return mModelMatrix;
}