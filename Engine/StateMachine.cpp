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
	this->state = SM_NOTHING;
	this->subState = SM_EMPTY;
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
	
	//if (action == GLFW_PRESS)
	//{
	//	switch (key)
	//	{
	//	case GLFW_KEY_G:
	//		this->state = SM_GRAB;
	//		break;
	//	case GLFW_KEY_R:
	//		this->state = SM_ROTATE;
	//		break;
	//	case GLFW_KEY_S:
	//		this->state = SM_SCALE;
	//		break;
	//		//if A has been pressed, state has to be changed: if ADD has been active, it has to change to NOTHING
	//	case GLFW_KEY_A:
	//		this->state == SM_ADD ? this->state = SM_NOTHING : this->state = SM_ADD;
	//		//if adding is done, state for adding meshLoaders should be empty
	//		if (this->state == SM_NOTHING) this->subState = SM_EMPTY;
	//		break;
	//	case GLFW_KEY_DELETE:
	//		this->state = SM_DELETE;
	//		if (m_target) DeleteObject();
	//		break;
	//	case GLFW_KEY_F:
	//		this->state = SM_FOCUS;
	//		break;
	//	case GLFW_KEY_Q:
	//		this->state = SM_RESTART_SCENE;
	//		this->m_camera->RestartCameraParameters();
	//		break;
	//	case GLFW_KEY_ESCAPE:
	//		this->state = SM_CLOSE_WINDOW;
	//		CloseWindow(window);
	//		break;
	//	case GLFW_KEY_X:
	//		this->subState = SM_X;
	//		break;
	//	case GLFW_KEY_Y:
	//		this->subState = SM_Y;
	//		break;
	//	case GLFW_KEY_Z:
	//		this->subState = SM_Z;
	//		break;
	//	case GLFW_KEY_1:
	//		this->subState = NO_1;
	//		break;
	//	case GLFW_KEY_2:
	//		this->subState = NO_2;
	//		break;
	//	case GLFW_KEY_3:
	//		this->subState = NO_3;
	//		break;
	//	case GLFW_KEY_4:
	//		this->subState = NO_4;
	//		break;
	//	case GLFW_KEY_5:
	//		this->subState = NO_5;
	//		break;
	//	case GLFW_KEY_6:
	//		this->subState = NO_6;
	//		break;
	//	case GLFW_KEY_7:
	//		this->subState = NO_7;
	//		break;
	//	case GLFW_KEY_8:
	//		this->subState = NO_8;
	//		break;
	//	//while moving camera preserve same state
	//	case GLFW_KEY_DOWN:
	//		break;
	//	case GLFW_KEY_UP:
	//		break;
	//	case GLFW_KEY_RIGHT:
	//		break;
	//	case GLFW_KEY_LEFT:
	//		break;
	//	default:
	//		this->state = SM_NOTHING;
	//		this->subState = SM_EMPTY;
	//		break;
	//	}
	//	this->followMouse = false;
	//	CheckTarget();
	//}
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

	//switch (button)
	//{
	////case GLFW_MOUSE_BUTTON_LEFT:
	////	if (action == GLFW_PRESS)
	////	{
	////		double xpos, ypos;
	////		glfwGetCursorPos(window, &xpos, &ypos);
	////		m_camera->Raycast(window, xpos, ypos);
	////		Ray* ray = m_camera->mRay;

	////		if (this->state == SM_ADD)
	////		{
	////			AddObject(ray);
	////		}
	////		//object transformation has been completed
	////		else if (this->state == SM_GRAB || this->state == SM_SCALE || this->state == SM_ROTATE)
	////		{
	////			if (!m_target) return;
	////			//this->target->ChangeColor(DEFAULT_OBJECT_COLOR);
	////			m_target->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
	////			m_target = nullptr;
	////			this->state = SM_NOTHING;
	////			this->subState = SM_EMPTY;
	////		}
	////		else
	////		{
	////			bool objectPicked = false;
	////			int pickedId = -1;
	////			//SortObjectsInScene();
	////			//std::cout << "OBJ IN SCENE SIZE: " << mObjectsInScene.size() << std::endl;
	////			
	////			glm::vec3 rs = ray->GetRayStart();
	////			glm::vec3 rdir = ray->GetRayDirection();
	////			std::vector<Object*> intersected;
	////			for (const auto& object : m_objectsInScene)
	////			{
	////				if (object.get()->getEditorCollider()->intersects(rs, rdir))
	////					intersected.emplace_back(object.get());
	////			}

	////			//sorts objects first by layer (starting from those that have higher priority layer)
	////			//and by distance (those closer to ray start have advantage)
	////			std::sort(intersected.begin(), intersected.end(),
	////				[&rs](Object* obj1, Object* obj2)
	////				{
	////					int layer1 = obj1->getEditorCollider()->getLayer();
	////					int layer2 = obj2->getEditorCollider()->getLayer();

	////					float distance1 = glm::distance(
	////						obj1->getComponent<Transform>()->getPosition(), rs);
	////					float distance2 = glm::distance(
	////						obj2->getComponent<Transform>()->getPosition(), rs);

	////					if (layer1 > layer2)
	////						return true;
	////					else if (layer1 < layer2)
	////						return false;
	////					else
	////					{
	////						if (distance1 <= distance2)
	////							return true;
	////						else
	////							return false;
	////					}
	////					return 
	////						glm::distance(obj1->getComponent<Transform>()->getPosition(), rs)
	////						< glm::distance(obj2->getComponent<Transform>()->getPosition(), rs);
	////				}
	////			);

	////			std::cout << "SORTED HITS (by priority descending):" << '\n';
	////			for (const auto* obj : intersected)
	////			{
	////				std::cout << obj->getName() << '\n';
	////			}
	////			if (intersected.size() > 0)
	////			{
	////				if (m_target && m_target != intersected[0])
	////					m_target->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
	////				m_target = intersected[0];
	////				m_target->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
	////				CalculateObjectPlane();
	////			}
	////			else
	////			{
	////				if (m_target)
	////					m_target->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
	////				m_target = nullptr;
	////			}


	////			if (m_target)
	////				std::cout << "HIT: " << m_target->getName() << '\n';
	////		}
	////	}
	////	break;

	//case GLFW_MOUSE_BUTTON_RIGHT:
	//	if (action == GLFW_PRESS)
	//	{
	//		//std::cout << "SHOULD ROTATE CAMERA \n";
	//		this->canRotateCamera = true;
	//		glfwGetCursorPos(window, &this->mousePosX, &this->mousePosY);
	//		std::cout << "PRESS (X, Y): (" << mousePosX << ", " << mousePosY << ")\n";
	//		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	//		glfwSetCursor(window, cursor);
	//	}
	//	else
	//	{
	//		glfwSetCursor(window, NULL);
	//		this->canRotateCamera = false;
	//	}
	//	break;
	//default:
	//	break;
	//}
	//if (m_target && this->state != SM_ADD)
	//{
	//	this->state = SM_NOTHING;
	//}
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
	/*if (mTarget && !this->canRotateCamera) return;

	else if (mTarget) camera.ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);*/

	/*if (m_target) m_camera->ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);*/

	//if (this->canRotateCamera)
	//{
	//	std::cout << "ROTATE/move (X, Y): (" << mouseX << ", " << mouseY << ")\n";
	//	this->m_camera->Rotate(/*window, */this->mousePosX, this->mousePosY, mouseX, mouseY);
	//	this->mousePosX = mouseX;
	//	this->mousePosY = mouseY;
	//}

	//switch (this->state)
	//{
	//case SM_GRAB:
	//	//std::cout << "SHOULD CHANGE TO GRAB" << '\n';
	//	Grab();
	//	break;
	//case SM_ROTATE:
	//	Rotate();
	//	break;
	//case SM_SCALE:
	//	Scale();
	//	break;
	//default:
	//	break;
	//}
}
void StateMachine::CheckTarget()
{
	//these states need to have target in order to be performed
	if (m_target == nullptr && 
		(this->state == SM_GRAB || this->state == SM_ROTATE ||
		this->state == SM_SCALE || this->state == SM_DELETE))
	{
		this->state = SM_NOTHING;
		this->followMouse = false;
	}
	else
	{
		this->followMouse = true;
	}
	return;
}

//void StateMachine::Scale()
//{
//	glm::vec3 translationVector = CalculateIntersectionPoint();
//	float scalingFactor = glm::distance(translationVector, m_target->getComponent<Transform>()->getPosition());
//	m_target->getComponent<Transform>()->setScale(scalingFactor);
//}

void StateMachine::AddObject(Ray* ray)
{
	MeshLoader* meshLoaderObj;
	/*std::cout << "MESH LOADERS SIZE AT ADD: " << mMeshLoaders.size() << std::endl;
	std::cout << "SUBSTATE: " << subState << std::endl;*/
	if (this->subState != SM_EMPTY && this->subState < mMeshLoaders.size())
	{
		meshLoaderObj = mMeshLoaders[this->subState].get();
	}
	else
	{
		meshLoaderObj = this->mMeshLoaders[0].get();
	}

	//std::cout << "MESH LOADERS IN STATE MACHINE: " << std::endl;
	/*for (int i = 0; i < mMeshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;
	}*/
	std::cout << "VERTICES: " << meshLoaderObj->vertices.size() << std::endl;
	
	glm::vec3 spawnPosition = (ray->GetRayStart() + ray->GetRayDirection() * 10.0f);
	auto objectTransform = Transform(spawnPosition,
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	
	auto objectRenderer = MeshRenderer(ResourceManager<Mesh>::addResource("ob1", std::make_unique<Mesh>(meshLoaderObj)), ResourceManager<Material>::addResource("ob1", std::make_unique<Material>(mShaderProgram)));
	
	
	auto obj = std::make_unique<Object>(std::move(objectTransform), std::move(objectRenderer));
	/*std::cout << "AFTER NEW MESH" << std::endl;
	std::cout << "BEFORE PUSH" << std::endl;*/
	m_objectsInScene.push_back(std::move(obj));
	//std::cout << "AFTER PUSH" << std::endl;
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


bool StateMachine::ShouldFollowMouse()
{
	return this->followMouse;
}

