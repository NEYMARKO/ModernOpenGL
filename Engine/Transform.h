#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class Object;

class Transform
{
private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f);

	Object* mParentObject{ nullptr };
	glm::mat4 mModelMatrix = glm::mat4(1.0f);

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

	void setParent(Object* parent);

	glm::mat4 getModelMatrix();
	glm::vec3 getRightVector();
	glm::vec3 getUpVector();
	glm::vec3 getForwardVector();
	glm::vec3 getPosition() { return mPosition; }
	glm::vec3 getEulerRotation() { return glm::degrees(glm::eulerAngles(mRotation)); }
	glm::quat getQuaternionRotation() { return mRotation; }	
	glm::vec3 getScale() { return mScale; }
};