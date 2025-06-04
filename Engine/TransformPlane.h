#pragma once
#include <glm/glm.hpp>

struct TransformPlane
{
	glm::vec3 m_normal;
	glm::vec3 m_origin;
	float m_D;

	TransformPlane(glm::vec3 origin) :
		m_origin{ origin }
	{}
	void calculatePlaneParameters(const glm::vec3& normal);
	void calculatePlaneParameters(const glm::vec3& normal, const glm::vec3& pointOnPlane);
	glm::vec3 calculateRayIntersectionPoint(const glm::vec3& mouseStartWorld, 
		const glm::vec3& mouseDirectionWorld);
	//Projects direction vector onto a plane
	void projectVectorToPlane(glm::vec3& vector);
	void projectPointToPlane(glm::vec3& point);
};