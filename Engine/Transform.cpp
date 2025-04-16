#include "Transform.h"

void Transform::translate(const glm::vec3& translation)
{
	mPosition += translation;
}

void Transform::rotate(const glm::vec3& axis, float angle)
{
	glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
	mQuatRotation = rotation * mQuatRotation;
}

void Transform::rotate(const glm::quat& rotation)
{
	mQuatRotation = glm::normalize(rotation * mQuatRotation);
}

void Transform::scale(const glm::vec3& scale)
{
	mScale += scale;
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
	mModelMatrix *= glm::toMat4(mQuatRotation);
	mModelMatrix = glm::scale(mModelMatrix, mScale);
}