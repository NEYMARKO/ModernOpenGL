#include "SelectedState.h"
#include "TransformState.h"
#include "GrabState.h"
#include "RotateState.h"
#include "CameraMoveState.h"
#include "CameraRotateState.h"
//#include "Object.h"
//#include "SceneEntity.h"
#include "Camera.h"
#include "Lighting.h"
#include "Ray.h"
#include "PhysicsWorld.h"
#include "StateMachine.h"

#define GLFW_HAND_CURSOR 0x00036004


StateMachine::StateMachine(Window* window, Camera* camera, SceneEntity* lightSource,
	std::vector<std::unique_ptr<SceneEntity>>& objectsInScene, PhysicsWorld* physicsWorld)
	: m_window{ window }, m_camera{ camera }, m_lightSource{ (Lighting*)lightSource },
	m_target { nullptr }, m_objectsInScene{ objectsInScene },
	m_physicsWorld { physicsWorld }
{
	m_activeState = std::make_unique<State>(this);
	this->m_camera = m_camera;
	this->mousePosX = this->m_camera->mWidth / 2;
	this->mousePosY = this->m_camera->mHeight / 2;
}

void StateMachine::update()
{
	m_activeState.get()->update();
}

void StateMachine::changeState()
{
	States newState = m_activeState.get()->getTransitionState();
	if (newState != States::NO_TRANSITION)
	{
		m_activeState.get()->exit();
		m_activeState.reset();
		if (!m_target)
			std::cout << "TARGET IS NULL\n";
		switch (newState)
		{
		case States::DEFAULT:
			m_activeState = std::make_unique<State>(this);
			break;
		case States::SELECTED:
			m_activeState = std::make_unique<SelectedState>(this, m_target);
			break;
		case States::GRAB:
			/*m_activeState = std::make_unique<GrabState>(this, m_camera, m_target->getComponent<Transform>());*/
			m_activeState = std::make_unique<GrabState>(this, m_camera, &(m_target->m_transform));
			break;
		case States::ROTATE:
			m_activeState = std::make_unique<RotateState>(this, m_camera, &(m_target->m_transform));
			break;
		case States::CAMERA_ROTATE:
			m_activeState = std::make_unique<CameraRotateState>(this, m_camera);
			break;
		case States::CAMERA_MOVE:
			m_activeState = std::make_unique<CameraMoveState>(this, m_camera);
			break;
		}
		m_activeState.get()->enter();
	}
}
void StateMachine::KeyboardPress(int key, int action, Camera* camera)
{	
	m_activeState.get()->onKeyboardPress(key, action);
	m_lastKey = key;
	changeState();
}

void StateMachine::MouseClick(Camera* camera, int button, int action)
{
	double xPos, yPos;
	glfwGetCursorPos(m_window->getGLFWWindow(), &xPos, &yPos);
	mousePosX = xPos;
	mousePosY = yPos;
	m_camera->ScreenToWorldCoordinates(xPos, yPos, mouseStartWorld, mouseDirectionWorld);
	m_activeState.get()->onMouseClick(mouseStartWorld, mouseDirectionWorld, button, action);
	changeState();	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//std::cout << "SHOULD CHECK FOR STATE CHANGE\n";
		m_camera->Raycast(m_window->getGLFWWindow(), xPos, yPos);
	}
}

void StateMachine::MouseMove(Camera* camera, double mouseX, double mouseY)
{
	
	if (m_activeState.get()->m_convertMouseTo3D)
	{
		//std::cout << "GIVING WORLD COORDINATES\n";
		m_camera->ScreenToWorldCoordinates(mouseX, mouseY, mouseStartWorld, mouseDirectionWorld);
		m_activeState.get()->onMouseMove(mouseStartWorld, mouseDirectionWorld);
	}
	else
	{
		//std::cout << "GIVING PIXEL COORDINATES\n";
		m_activeState.get()->onMouseMove(mouseX, mouseY);
	}
	mousePosX = mouseX;
	mousePosY = mouseY;
}

//void StateMachine::Scale()
//{
//	glm::vec3 translationVector = CalculateIntersectionPoint();
//	float scalingFactor = glm::distance(translationVector, m_target->getComponent<Transform>()->getPosition());
//	m_target->getComponent<Transform>()->setScale(scalingFactor);
//}

void StateMachine::AddObject(Ray* ray)
{
	
}
void StateMachine::DeleteObject()
{

}
void StateMachine::CloseWindow()
{
	m_window->closeWindow();
}

