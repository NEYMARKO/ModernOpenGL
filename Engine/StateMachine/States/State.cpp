#include <iostream>
#include <algorithm>

#include "../StateMachine.h"
#include "../../Camera.h"
#include "../../Lighting.h"
#include "../../Ray.h"
#include "../../Object.h"
#include "../../Components/Transform.h"
#include "../../Components/MeshRenderer.h"
#include "../../EditorCollider.h"
#include "../../PhysicsWorld.h"
#include "./State.h"

#define DEFAULT_OBJECT_COLOR glm::vec3(0.862745f, 0.862745f, 0.862745f)
#define SELECTED_OBJECT_COLOR glm::vec3(0.0f, 1.0f, 0.0f)

void State::onKeyboardPress(int key, int action)
{

	//std::cout << "STATE CONVERT TO 3D: " << (m_convertMouseTo3D == true ? "TRUE" : "FALSE") << "\n";
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
				/*m_stateMachine->m_target->getComponent<MeshRenderer>()->
					changeColor(DEFAULT_OBJECT_COLOR);*/
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

			/*glm::vec3 startHelper;
			glm::vec3 dirHelper;

			glm::mat4 transformInverse;*/
			BulletRayHit physicsHit = m_stateMachine->m_physicsWorld->getIntersection(start, start + dir * m_stateMachine->m_rayLen);
			m_stateMachine->m_rbPickPos = btVector3(physicsHit.point.x, physicsHit.point.y, physicsHit.point.z);
			for (const auto& object : m_stateMachine->m_objectsInScene)
			{
				/*transformInverse = glm::inverse(object->getComponent<Transform>()->getModelMatrix());
				startHelper = glm::vec3(transformInverse * glm::vec4(start, 1));
				dirHelper = glm::normalize(glm::vec3(transformInverse * glm::vec4(dir, 0)));*/
				if (object.get()->m_editorCollider.intersects(start, dir, intersectionPoint))
					hits.emplace_back(Hit{ object.get(), intersectionPoint });
			}
			/*if (physicsHit.obj != nullptr)
			{
				hits.emplace_back(physicsHit);
			}*/
			sortObjects(hits, start);
			std::cout << "SORTED HITS (by priority descending):" << '\n';
			for (const auto& hit: hits)
			{
				std::cout << hit.obj->m_name << '\n';
			}
			if (hits.size() > 0)
			{
				// check whether rigidbody will be modified or SceneEntity
				if (glm::distance(start, physicsHit.point) < glm::distance(start, hits[0].point))
					m_activeRigid = true;
				else m_activeRigid = false;
			}
			updateSelection(hits, physicsHit.rb);
			//std::cout << "Hit distance: " << glm::distance(hits[0].point, start) << "\n";
			/*bool lightIntersects = m_stateMachine->m_lightSource->m_editorCollider.intersects(start, dir, intersectionPoint);
			std::cout << "LIGHT INTERSECTS: " << (lightIntersects ? "TRUE" : "FALSE") << "\n";*/
			/*if (!m_stateMachine->m_target)
			{
				if (lightIntersects)
					std::cout << "LIGHT HIT\n";
			}
			else
			{
				if (lightIntersects && (glm::distance(intersectionPoint, start) < glm::distance(hits[0].point, start)))
					std::cout << "LIGHT HIT BEFORE OTHER OBJECT\n";
			}*/

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
			/*m_stateMachine->m_target->getComponent<MeshRenderer>()->
				changeColor(DEFAULT_OBJECT_COLOR);*/
			m_stateMachine->m_target = nullptr;
		}

		if (m_stateMachine->m_rbTarget) m_stateMachine->m_rbTarget = nullptr;
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
			//std::cout << "HIT1: " << hit1.obj->getName() << ", HIT2: " << hit2.obj->getName() << "\n";
			int layer1 = hit1.obj->m_editorCollider.m_layer;
			int layer2 = hit2.obj->m_editorCollider.m_layer;
			//std::cout << "LAYER1: " << layer1 << " LAYER2: " << layer2 << "\n";
			float distance1 = glm::distance(hit1.point, start);
			float distance2 = glm::distance(hit2.point, start);

			//std::cout << "DISTANCE1: " << distance1 << " DISTANCE2: " << distance2 << "\n";
			if (layer1 != layer2)
				return layer1 > layer2;
			
			return distance1 < distance2;
		}
	);
}

void State::updateSelection(const std::vector<Hit>& hits, btRigidBody* rb)
{
	SceneEntity* currentSelection = m_stateMachine->m_target;
	//ray has hit something - objects is bound to have atleast 1 element
	if (hits.size() > 0 && !m_activeRigid)
	{
		m_stateMachine->m_rbTarget = nullptr;
		//there isn't active selection
		if (!currentSelection)
		{
			m_stateMachine->m_target = hits[0].obj;
			/*m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);*/
			//CalculateObjectPlane();
			m_transitionState = States::SELECTED;
			return;
		}
		//There already exists active selection, but it isn't same as hit
		else if (currentSelection != hits[0].obj)
		{
			/*currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);*/
			m_stateMachine->m_target = hits[0].obj;
			/*m_stateMachine->m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);*/
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
	else if (rb != nullptr)
	{
		if (m_stateMachine->m_rbTarget && m_stateMachine->m_rbTarget == rb)
		{
			m_transitionState = States::NO_TRANSITION;
		}
		//rb target either non existent or not equal to rb - must be updated
		else
		{
			m_stateMachine->m_rbTarget = rb;
			m_transitionState = States::SELECTED;
		}
	}
	//ray missed everything
	else
	{
		m_stateMachine->m_rbTarget = nullptr;
		std::cout << "RAY MISSED EVERYTHING, RETURNING TO DEFAULT STATE\n";
		if (currentSelection)
			/*currentSelection->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);*/
		m_stateMachine->m_target = nullptr;
		m_transitionState = States::DEFAULT;
	}
}