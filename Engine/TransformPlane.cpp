#include "TransformPlane.h"

void TransformPlane::calculatePlaneParameters(glm::vec3& normal)
{
	calculatePlaneParameters(normal, m_origin);
}

void TransformPlane::calculatePlaneParameters(glm::vec3& normal,
	const glm::vec3& pointOnPlane)
{
	m_normal = normal;
	m_D = -glm::dot(m_normal, pointOnPlane);
}

glm::vec3 TransformPlane::calculateRayIntersectionPoint(const glm::vec3& mouseStartWorld,
	const glm::vec3& mouseDirectionWorld)
{
	float t = (-glm::dot(m_normal, mouseStartWorld) - m_D) 
		/ glm::dot(m_normal, mouseDirectionWorld);
	return mouseStartWorld + mouseDirectionWorld * t;
}

void TransformPlane::projectPointToPlane(glm::vec3& point)
{
	point = point - m_normal * (glm::dot(m_normal, point) + m_D);
}

void TransformPlane::projectVectorToPlane(glm::vec3& vector)
{
	vector = vector - (glm::dot(vector, m_normal) / 
		glm::dot(m_normal, m_normal)) * m_normal;
}
