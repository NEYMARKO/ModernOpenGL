#include <iostream>
#include <algorithm>

#include "OpenGLIncludes.h"
#include "StateMachine.h"
#include "Camera.h"
#include "Ray.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "EditorCollider.h"
#include "State.h"

#define DEFAULT_OBJECT_COLOR glm::vec3(0.862745f, 0.862745f, 0.862745f)
#define SELECTED_OBJECT_COLOR glm::vec3(0.0f, 1.0f, 0.0f)

void State::onMouseClick(GLFWwindow* window, int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		if (action == GLFW_PRESS)
		{
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);
			m_stateMachine->m_camera->Raycast(window, xPos, yPos);

			Ray* ray = m_stateMachine->m_camera->mRay;
			glm::vec3 rayStart = ray->GetRayStart();
			glm::vec3 rayDir = ray->GetRayDirection();
			std::vector<Object*> intersected;

			for (const auto& object : m_stateMachine->m_objectsInScene)
			{
				if (object.get()->getEditorCollider()->intersects(rayStart, rayDir))
					intersected.emplace_back(object.get());
			}

			sortObjects(intersected, rayStart);
			std::cout << "SORTED HITS (by priority descending):" << '\n';
			for (const auto* obj : intersected)
			{
				std::cout << obj->getName() << '\n';
			}
			updateSelection(intersected);
			if (m_stateMachine->m_target)
				std::cout << "HIT: " << m_stateMachine->m_target->getName() << '\n';
		}
}

void State::sortObjects(std::vector<Object*>& objects, const glm::vec3& start)
{
	std::sort(objects.begin(), objects.end(),
		[&start](Object* obj1, Object* obj2)
		{
			int layer1 = obj1->getEditorCollider()->getLayer();
			int layer2 = obj2->getEditorCollider()->getLayer();

			float distance1 = glm::distance(
				obj1->getComponent<Transform>()->getPosition(), start);
			float distance2 = glm::distance(
				obj2->getComponent<Transform>()->getPosition(), start);

			if (layer1 > layer2)
				return true;
			else if (layer1 < layer2)
				return false;
			//objects are in same layer - check next condition: distance from ray start
			else
			{
				if (distance1 <= distance2)
					return true;
				else
					return false;
			}
		}
	);
}

void State::updateSelection(const std::vector<Object*>& objects)
{
	Object* currentSelection = m_stateMachine->m_target;
	if (objects.size() > 0)
	{
		if (currentSelection && currentSelection != objects[0])
			currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
		m_stateMachine->m_target = objects[0];
		m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
		//CalculateObjectPlane();
		m_transitionState = States::SELECTED;
	}
	else
	{
		if (currentSelection)
			currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
		m_stateMachine->m_target = nullptr;
		m_transitionState = States::DEFAULT;
	}
}