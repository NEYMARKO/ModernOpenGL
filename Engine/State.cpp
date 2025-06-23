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
			if (m_stateMachine->m_target)
			{
				m_stateMachine->m_target->getComponent<MeshRenderer>()->
					changeColor(DEFAULT_OBJECT_COLOR);
				m_stateMachine->m_target = nullptr;
			}
			m_transitionState = States::CAMERA_MOVE;
		}
		else if (key == GLFW_KEY_ESCAPE)
			m_stateMachine->CloseWindow();
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
			std::vector<Hit> hits;
			glm::vec3 intersectionPoint;
			for (const auto& object : m_stateMachine->m_objectsInScene)
			{
				if (object.get()->getEditorCollider()->intersects(start, dir, intersectionPoint))
					hits.emplace_back(Hit{ object.get(), intersectionPoint });
			}

			sortObjects(hits, start);
			std::cout << "SORTED HITS (by priority descending):" << '\n';
			for (const auto hit: hits)
			{
				std::cout << hit.obj->getName() << '\n';
			}
			updateSelection(hits);
			/*if (m_stateMachine->m_target)
				std::cout << "HIT: " << m_stateMachine->m_target->getName() << '\n';*/
		}
	}
	else
	{
		//This code can't be put in SelectedState::exit() because that will get triggered
		//both when going to camera move/rotate (as wanted), and when going to transform state
		//(which will then perform something over nullptr - unwanted behaviour)
		if (m_stateMachine->m_target)
		{
			m_stateMachine->m_target->getComponent<MeshRenderer>()->
				changeColor(DEFAULT_OBJECT_COLOR);
			m_stateMachine->m_target = nullptr;
		}
		//std::cout << "RM CLICKED\n";

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

void State::sortObjects(std::vector<Hit>& hits, const glm::vec3& start)
{
	std::sort(hits.begin(), hits.end(),
		[&start](Hit hit1, Hit hit2)
		{
			int layer1 = hit1.obj->getEditorCollider()->getLayer();
			int layer2 = hit2.obj->getEditorCollider()->getLayer();

			float distance1 = glm::distance(hit1.point, start);
			float distance2 = glm::distance(hit2.point, start);

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

void State::updateSelection(const std::vector<Hit>& hits)
{
	Object* currentSelection = m_stateMachine->m_target;
	//ray has hit something - objects is bound to have atleast 1 element
	if (hits.size() > 0)
	{
		//there isn't active selection
		if (!currentSelection)
		{
			m_stateMachine->m_target = hits[0].obj;
			m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
			//CalculateObjectPlane();
			m_transitionState = States::SELECTED;
			return;
		}
		//There already exists active selection, but it isn't same as hit
		else if (currentSelection != hits[0].obj)
		{
			currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
			m_stateMachine->m_target = hits[0].obj;
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