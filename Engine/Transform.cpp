#include "Transform.h"

Transform::Transform() :
	m_position{ 0.0f, 0.0f, 0.0f },
	m_rotation{ glm::quat() },
	m_scale{ 1.0f, 1.0f, 1.0f }
{
}

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: m_position{ position }, m_rotation{ rotation }, m_scale{ scale }
{
	//position, rotation and scale have changed, model matrix needs to be updated
	mDirty = true;
}

void Transform::translate(const glm::vec3& translation)
{
	m_position += translation;
	mDirty = true;
}

void Transform::rotate(const glm::vec3& axis, float angle)
{
	glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
	m_rotation = glm::normalize(rotation * m_rotation);
	mDirty = true;
}

void Transform::rotate(const glm::quat& rotation)
{
	m_rotation = glm::normalize(rotation * m_rotation);
	mDirty = true;
}

void Transform::scale(const glm::vec3& scale)
{
	m_scale += scale;
	mDirty = true;
}

void Transform::scale(float scale)
{
	m_scale += glm::vec3(scale, scale, scale);
	mDirty = true;
}

void Transform::setPosition(const glm::vec3& position)
{
	m_position = position;
	mDirty = true;
}

void Transform::setRotation(const glm::quat& rotation)
{
	m_rotation = rotation;
	mDirty = true;
}

void Transform::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	mDirty = true;
}

void Transform::setScale(float scale)
{
	m_scale = glm::vec3(scale, scale, scale);
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
	//m_modelMatrix = glm::mat4(1.0f);
	//m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	////m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(0.0f), glm::vec3(0.0, 0.0, 0.0));
	//m_modelMatrix *= glm::toMat4(m_rotation);
	//m_modelMatrix = glm::scale(m_modelMatrix, m_scale);


	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position) *
		glm::toMat4(m_rotation) *
		glm::scale(glm::mat4(1.0f), m_scale);

	mDirty = false;
}

glm::vec3 Transform::getRightVector()
{
	return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::getUpVector()
{
	return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Transform::getForwardVector()
{
	return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

btQuaternion Transform::getBulletQuat()
{
	return btQuaternion(m_rotation.x,
		m_rotation.y,
		m_rotation.z,
		m_rotation.w
	);
}

glm::mat4 Transform::getModelMatrix()
{
	if (mDirty)
		updateModelMatrix();
	return m_modelMatrix;
}