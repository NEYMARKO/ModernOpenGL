#include "RotateState.h"
#include <glm/gtx/string_cast.hpp>

#define EPSILON 0.01f

glm::vec3 RotateState::projectPointToCircle(const glm::vec3& point)
{
	return m_transformPlane.m_origin + 
		glm::normalize(point - m_transformPlane.m_origin) * m_radius;
}

void RotateState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	if (!m_trackingMouse)
	{
		TransformState::onMouseMove(mouseStartWorld, mouseDirectionWorld);
		return;
	}
	glm::vec3 planeIntersectionPoint = m_transformPlane.calculateRayIntersectionPoint(mouseStartWorld, mouseDirectionWorld);
	//std::cout << "INTERSECTION: " << glm::to_string(planeIntersectionPoint) << "\n";
	glm::vec3 pointOnCircle = projectPointToCircle(planeIntersectionPoint);
	//check if old intersection is vector.zero 
	
	if (glm::length(m_oldIntersection) < EPSILON * EPSILON
		|| glm::distance(m_oldIntersection, pointOnCircle) <= EPSILON)
	{
		m_oldIntersection = pointOnCircle;
		return;
	}
	//std::cout << "DISTANCE: " << glm::distance(m_oldIntersection, pointOnCircle) << "\n";
	//std::cout << "EPSILON: " << EPSILON << "\n";
	/*std::cout << "OLD: " << glm::to_string(m_oldIntersection) << "\n";
	std::cout << "POINT ON CIRCLE: " << glm::to_string(pointOnCircle) << "\n";*/
	glm::vec3 newVector = glm::normalize(m_oldIntersection - m_transformPlane.m_origin);
	glm::vec3 oldVector = glm::normalize(pointOnCircle - m_transformPlane.m_origin);
	glm::vec3 crossProduct = glm::cross(newVector, oldVector);
	float angle = glm::atan(glm::dot(crossProduct, m_transformPlane.m_normal), glm::dot(newVector, oldVector));
	/*float angle = glm::acos(glm::dot(glm::normalize(m_oldIntersection - m_transformPlane.m_origin)
		, glm::normalize(pointOnCircle - m_transformPlane.m_origin)));*/
	//std::cout << "ANGLE: " << angle << "\n";
	//transform around all axis equally
	if (m_transformAxis == TransformAxis::NONE)
	{
		glm::quat newRotation = glm::angleAxis(angle, m_transformPlane.m_normal);
		m_selectedTransform->rotate(newRotation);
	}
	else
	{
		glm::quat newRotation = glm::angleAxis(angle, m_infiniteAxis);
		m_selectedTransform->rotate(newRotation);
	}
	m_oldIntersection = pointOnCircle;
	
}