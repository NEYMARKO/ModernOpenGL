#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform
{
private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::mat4 mModelMatrix = glm::mat4(1.0f);

	bool mDirty{ false };

public:
	Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	void translate(const glm::vec3& translation);
	void rotate(const glm::vec3& axis, float angle);
	void rotate(const glm::quat& rotation);
	void scale(const glm::vec3& scale);

	void setPosition(const glm::vec3& position) { mPosition = position; }
	void setRotation(const glm::quat& rotation) { mRotation = rotation; }
	void setScale(const glm::vec3& scale) { mScale = scale; }

	void updateModelMatrix();

	glm::mat4 getModelMatrix();

	/*glm::vec3 getRightVector() { return glm::normalize(glm::vec3(mModelMatrix[0])); }
	glm::vec3 getUpVector() { return glm::normalize(glm::vec3(mModelMatrix[1])); }
	glm::vec3 getForwardVector() { return glm::normalize(glm::vec3(mModelMatrix[2]));}*/
	glm::vec3 getRightVector();
	glm::vec3 getUpVector();
	glm::vec3 getForwardVector();
	glm::vec3 getPosition() { return mPosition; }
	glm::vec3 getEulerRotation() { return glm::degrees(glm::eulerAngles(mRotation)); }
	glm::quat getQuaternionRotation() { return mRotation; }	
	glm::vec3 getScale() { return mScale; }
};