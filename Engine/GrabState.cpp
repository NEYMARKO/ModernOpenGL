#include "Object.h"
#include "Transform.h"
#include "GrabState.h"

glm::vec3 GrabState::projectPointToVector(const glm::vec3& point, const glm::vec3& vector)
{
	glm::vec3 originToPoint = point - m_transformPlane.m_origin;
	return m_transformPlane.m_origin + glm::dot(originToPoint, vector) /
		glm::dot(vector, vector) * vector;
}

void GrabState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	if (!m_trackingMouse)
	{
		TransformState::onMouseMove(mouseStartWorld, mouseDirectionWorld);
		return;
	}
	/*std::cout << "MOUSE MOVING IN GRAB\n";*/
	glm::vec3 planeIntersectionPoint = m_transformPlane.calculateRayIntersectionPoint(mouseStartWorld, mouseDirectionWorld);
	if (m_transformAxis == TransformAxis::NONE)
	{
		m_selectedTransform->setPosition(planeIntersectionPoint);
	}
	else
	{
		float movedDistance = glm::distance(
			projectPointToVector(planeIntersectionPoint, m_projectedAxis), m_transformPlane.m_origin);
		std::cout << "MOVED DISTANCE: " << movedDistance << "\n";
		float direction = glm::dot(glm::normalize(planeIntersectionPoint - m_transformPlane.m_origin), glm::normalize(m_projectedAxis));
		std::cout << "DIRECTION: " << direction << "\n";
		m_selectedTransform->setPosition(m_transformPlane.m_origin + m_infiniteAxis * movedDistance * direction);
	}
}