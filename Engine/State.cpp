#include <iostream>
#include <algorithm>

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


void State::onKeyboardPress(int key, int action)
{
	//NO MATTER WHAT STATE WE ARE CURRENTLY IN, IF THESE BUTTONS GET PRESSEED,
	//CAMERA HAS TO GET MOVED
	if (action == GLFW_PRESS /*&& !m_movingCamera*/)
	{
		if (key == GLFW_KEY_UP ||
			key == GLFW_KEY_DOWN ||
			key == GLFW_KEY_LEFT ||
			key == GLFW_KEY_RIGHT ||
			key == GLFW_KEY_SPACE ||
			key == GLFW_KEY_LEFT_CONTROL)
		{
			/*m_movingCamera = true;*/
			m_transitionState = States::CAMERA_MOVE;
		}
	}
}

void State::onMouseClick(const glm::vec3& start, const glm::vec3& dir, 
	int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			//std::cout << "ENTERED STATE MOUSE CLICK\n";
			std::vector<Object*> intersected;

			for (const auto& object : m_stateMachine->m_objectsInScene)
			{
				if (object.get()->getEditorCollider()->intersects(start, dir))
					intersected.emplace_back(object.get());
			}

			sortObjects(intersected, start);
			//std::cout << "SORTED HITS (by priority descending):" << '\n';
			/*for (const auto* obj : intersected)
			{
				std::cout << obj->getName() << '\n';
			}*/
			updateSelection(intersected);
			/*if (m_stateMachine->m_target)
				std::cout << "HIT: " << m_stateMachine->m_target->getName() << '\n';*/
		}
	}
	else
	{
		m_stateMachine->m_target = nullptr;
		std::cout << "RM CLICKED\n";

		//TODO: AVOID CHANGING STATE IF CAMERA_ROTATE_STATE CALLED THIS CODE - IT SHOULD REMAIN IN 
		//THAT SAME STATE - HOW TO CHECK WHETHER CAMERA_ROTATE_STATE CALLED?
		if (action == GLFW_PRESS)
		{
			m_transitionState = States::CAMERA_ROTATE;
		}
		//RM has been released - stop with camera rotation
		else
		{
			m_transitionState = DEFAULT;
		}
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
	//ray has hit something - objects is bound to have atleast 1 element
	if (objects.size() > 0)
	{
		//there isn't active selection
		if (!currentSelection)
		{
			m_stateMachine->m_target = objects[0];
			m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
			//CalculateObjectPlane();
			m_transitionState = States::SELECTED;
			return;
		}
		//There already exists active selection, but it isn't same as hit
		else if (currentSelection != objects[0])
		{
			currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
			m_stateMachine->m_target = objects[0];
			m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
			m_transitionState = States::SELECTED;
			
		}
		//currently active selection is same as the hit
		else
		{
			m_transitionState = States::NO_TRANSITION;
			std::cout << "STAYED IN SAME STATE\n";
			return;
		}
		
	}
	//ray missed everything
	else
	{
		std::cout << "WILL BE IN DEFAULT STATE\n";
		if (currentSelection)
			currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
		m_stateMachine->m_target = nullptr;
		m_transitionState = States::DEFAULT;
	}
}