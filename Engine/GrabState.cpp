#include "Object.h"
#include "Transform.h"
#include "GrabState.h"

void GrabState::onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld)
{
	if (!m_trackingMouse)
	{
		TransformState::onMouseMove(mouseStartWorld, mouseDirectionWorld);
		return;
	}
	/*std::cout << "MOUSE MOVING IN GRAB\n";*/
	glm::vec3 translationVector = m_transformPlane.calculateRayIntersectionPoint(mouseStartWorld, mouseDirectionWorld);
	m_selectedTransform->setPosition(translationVector);
}