#include <algorithm> //necessary for sorting vector hits
//#include "State.h"
#include "GrabState.h"
#include "RotateState.h"
#include "TransformState.h"
#include "CameraMoveState.h"
#include "CameraRotateState.h"
#include "SelectedState.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Ray.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Material.h"
#include "PhysicsWorld.h"
#include "EditorCollider.h"
#include "StateMachine.h"
#include "glm/gtx/string_cast.hpp"
#define GLFW_HAND_CURSOR 0x00036004
#define DEFAULT_OBJECT_COLOR glm::vec3(0.862745f, 0.862745f, 0.862745f)
#define SELECTED_OBJECT_COLOR glm::vec3(0.0f, 1.0f, 0.0f)

StateMachine::StateMachine(Camera* m_camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Object>>& objectsInScene, PhysicsWorld* physicsWorld)
	: m_target{ nullptr }, m_objectsInScene { objectsInScene }, mMeshLoaders{ meshLoaders },
	m_physicsWorld { physicsWorld }
{
	m_activeState = std::make_unique<State>(this);
	this->m_camera = m_camera;
	this->mousePosX = this->m_camera->mWidth / 2;
	this->mousePosY = this->m_camera->mHeight / 2;
}

void StateMachine::AddShaderPrograms(Shader* shader, Shader* boxShader)
{
	mShaderProgram = shader;
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
		switch (newState)
		{
		case States::DEFAULT:
			m_activeState = std::make_unique<State>(this);
			break;
		case States::SELECTED:
			m_activeState = std::make_unique<SelectedState>(this, m_target);
			break;
		case States::GRAB:
			m_activeState = std::make_unique<GrabState>(this, m_camera, m_target->getComponent<Transform>());
			break;
		case States::ROTATE:
			m_activeState = std::make_unique<RotateState>(this, m_camera, m_target->getComponent<Transform>());
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
void StateMachine::KeyboardPress(GLFWwindow* window, int key, int action, Camera* camera)
{	
	m_activeState.get()->onKeyboardPress(key, action);
	m_lastKey = key;
	changeState();
}

void StateMachine::MouseClick(GLFWwindow* window, Camera* camera, int button, int action)
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	mousePosX = xPos;
	mousePosY = yPos;
	m_camera->ScreenToWorldCoordinates(xPos, yPos, mouseStartWorld, mouseDirectionWorld);
	m_activeState.get()->onMouseClick(mouseStartWorld, mouseDirectionWorld, button, action);
	changeState();	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//std::cout << "SHOULD CHECK FOR STATE CHANGE\n";
		m_camera->Raycast(window, xPos, yPos);
	}
}

void StateMachine::MouseMove(GLFWwindow* window, Camera* camera, double mouseX, double mouseY)
{
	
	if (m_activeState.get()->m_convertMouseTo3D)
	{
		m_camera->ScreenToWorldCoordinates(mouseX, mouseY, mouseStartWorld, mouseDirectionWorld);
		m_activeState.get()->onMouseMove(mouseStartWorld, mouseDirectionWorld);
	}
	else
	{
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
	//MeshLoader* meshLoaderObj;
	///*std::cout << "MESH LOADERS SIZE AT ADD: " << mMeshLoaders.size() << std::endl;
	//std::cout << "SUBSTATE: " << subState << std::endl;*/
	//if (this->subState != SM_EMPTY && this->subState < mMeshLoaders.size())
	//{
	//	meshLoaderObj = mMeshLoaders[this->subState].get();
	//}
	//else
	//{
	//	meshLoaderObj = this->mMeshLoaders[0].get();
	//}

	////std::cout << "MESH LOADERS IN STATE MACHINE: " << std::endl;
	///*for (int i = 0; i < mMeshLoaders.size(); i++)
	//{
	//	std::cout << "MESH LOADER " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;
	//}*/
	//std::cout << "VERTICES: " << meshLoaderObj->vertices.size() << std::endl;
	//
	//glm::vec3 spawnPosition = (ray->GetRayStart() + ray->GetRayDirection() * 10.0f);
	//auto objectTransform = Transform(spawnPosition,
	//	glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	//
	//auto objectRenderer = MeshRenderer(ResourceManager<Mesh>::addResource("ob1", std::make_unique<Mesh>(meshLoaderObj)), ResourceManager<Material>::addResource("ob1", std::make_unique<Material>(mShaderProgram)));
	//
	//
	//auto obj = std::make_unique<Object>(std::move(objectTransform), std::move(objectRenderer));
	///*std::cout << "AFTER NEW MESH" << std::endl;
	//std::cout << "BEFORE PUSH" << std::endl;*/
	//m_objectsInScene.push_back(std::move(obj));
	////std::cout << "AFTER PUSH" << std::endl;
}
void StateMachine::DeleteObject()
{
	//int position = 0;

	//for (position; position < this->mObjectsInScene.size() && 
	//	this->target->id != this->mObjectsInScene[position]->id; position++)
	//{
	//}

	//this->mObjectsInScene.erase(this->mObjectsInScene.begin() + position);
	//
	///*for (int i = targetPos; i < this->objectsInScene.size(); i++)
	//{
	//	this->objectsInScene[i]->id--;
	//}*/

	//delete this->target;
	//this->state = NOTHING;
}
void StateMachine::CloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

