#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <btBulletDynamicsCommon.h>
#include "Component.h"

class Transform : public Component
{
private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	bool mDirty{ false };

public:

	Transform();
	Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

	void translate(const glm::vec3& translation);
	void rotate(const glm::vec3& axis, float angle);
	void rotate(const glm::quat& rotation);
	void scale(const glm::vec3& scale);
	void scale(float scale);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::quat& rotation);
	void setScale(const glm::vec3& scale);
	void setScale(float scale);
	void updateModelMatrix();

	glm::mat4 getModelMatrix();
	glm::vec3 getRightVector();
	glm::vec3 getUpVector();
	glm::vec3 getForwardVector();
	glm::vec3 getPosition() { return m_position; }
	btVector3 getBulletPosition()
	{
		return btVector3(m_position.x, m_position.y, m_position.z);
	}
	glm::vec3 getEulerRotation() { return glm::degrees(glm::eulerAngles(m_rotation)); }
	glm::quat getQuaternionRotation() { return m_rotation; }	
	btQuaternion getBulletQuat();
	glm::quat bulletToGlmQuat(const btQuaternion& quat)
	{
		return glm::quat(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
	}
	glm::vec3 getScale() { return m_scale; }
};