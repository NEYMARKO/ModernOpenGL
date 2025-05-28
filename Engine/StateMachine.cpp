#include <algorithm> //necessary for sorting vector hits
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

StateMachine::StateMachine(Camera* camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Object>>& objectsInScene, PhysicsWorld* physicsWorld)
	: mTarget{ nullptr }, mObjectsInScene { objectsInScene }, mMeshLoaders{ meshLoaders },
	m_physicsWorld { physicsWorld }
{
	this->state = NOTHING;
	this->subState = EMPTY;
	this->camera = camera;
	this->mousePosX = this->camera->mWidth / 2;
	this->mousePosY = this->camera->mHeight / 2;
}

void StateMachine::AddShaderPrograms(Shader* shader, Shader* boxShader)
{
	mShaderProgram = shader;
}
void StateMachine::ChangeState(GLFWwindow* window, const int key, const int action, Camera& camera)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_G:
			this->state = GRAB;
			break;
		case GLFW_KEY_R:
			this->state = ROTATE;
			break;
		case GLFW_KEY_S:
			this->state = SCALE;
			break;
			//if A has been pressed, state has to be changed: if ADD has been active, it has to change to NOTHING
		case GLFW_KEY_A:
			this->state == ADD ? this->state = NOTHING : this->state = ADD;
			//if adding is done, state for adding meshLoaders should be empty
			if (this->state == NOTHING) this->subState = EMPTY;
			break;
		case GLFW_KEY_DELETE:
			this->state = DELETE;
			if (mTarget) DeleteObject();
			break;
		case GLFW_KEY_F:
			this->state = FOCUS;
			break;
		case GLFW_KEY_Q:
			this->state = RESTART_SCENE;
			this->camera->RestartCameraParameters();
			break;
		case GLFW_KEY_ESCAPE:
			this->state = CLOSE_WINDOW;
			CloseWindow(window);
			break;
		case GLFW_KEY_X:
			this->subState = X;
			break;
		case GLFW_KEY_Y:
			this->subState = Y;
			break;
		case GLFW_KEY_Z:
			this->subState = Z;
			break;
		case GLFW_KEY_1:
			this->subState = NO_1;
			break;
		case GLFW_KEY_2:
			this->subState = NO_2;
			break;
		case GLFW_KEY_3:
			this->subState = NO_3;
			break;
		case GLFW_KEY_4:
			this->subState = NO_4;
			break;
		case GLFW_KEY_5:
			this->subState = NO_5;
			break;
		case GLFW_KEY_6:
			this->subState = NO_6;
			break;
		case GLFW_KEY_7:
			this->subState = NO_7;
			break;
		case GLFW_KEY_8:
			this->subState = NO_8;
			break;
		//while moving camera preserve same state
		case GLFW_KEY_DOWN:
			break;
		case GLFW_KEY_UP:
			break;
		case GLFW_KEY_RIGHT:
			break;
		case GLFW_KEY_LEFT:
			break;
		default:
			this->state = NOTHING;
			this->subState = EMPTY;
			break;
		}
		this->followMouse = false;
		CheckTarget();
	}
}

void StateMachine::MouseClick(GLFWwindow* window, Camera& camera, int button, int action)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			camera.Raycast(window, xpos, ypos);
			Ray* ray = camera.mRay;

			if (this->state == ADD)
			{
				AddObject(ray);
			}
			//object transformation has been completed
			else if (this->state == GRAB || this->state == SCALE || this->state == ROTATE)
			{
				if (!mTarget) return;
				//this->target->ChangeColor(DEFAULT_OBJECT_COLOR);
				mTarget->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
				mTarget = nullptr;
				this->state = NOTHING;
				this->subState = EMPTY;
			}
			else
			{
				bool objectPicked = false;
				int pickedId = -1;
				//SortObjectsInScene();
				//std::cout << "OBJ IN SCENE SIZE: " << mObjectsInScene.size() << std::endl;
				
				glm::vec3 rs = ray->GetRayStart();
				glm::vec3 rdir = ray->GetRayDirection();
				std::vector<Object*> intersected;
				for (const auto& object : mObjectsInScene)
				{
					if (object.get()->getEditorCollider()->intersects(rs, rdir))
						intersected.emplace_back(object.get());
				}

				//sorts objects first by layer (starting from those that have higher priority layer)
				//and by distance (those closer to ray start have advantage)
				std::sort(intersected.begin(), intersected.end(),
					[&rs](Object* obj1, Object* obj2)
					{
						int layer1 = obj1->getEditorCollider()->getLayer();
						int layer2 = obj2->getEditorCollider()->getLayer();

						float distance1 = glm::distance(
							obj1->getComponent<Transform>()->getPosition(), rs);
						float distance2 = glm::distance(
							obj2->getComponent<Transform>()->getPosition(), rs);

						if (layer1 > layer2)
							return true;
						else if (layer1 < layer2)
							return false;
						else
						{
							if (distance1 <= distance2)
								return true;
							else
								return false;
						}
						return 
							glm::distance(obj1->getComponent<Transform>()->getPosition(), rs)
							< glm::distance(obj2->getComponent<Transform>()->getPosition(), rs);
					}
				);

				std::cout << "SORTED HITS (by priority descending):" << '\n';
				for (const auto* obj : intersected)
				{
					std::cout << obj->getName() << '\n';
				}
				if (intersected.size() > 0)
				{
					if (mTarget && mTarget != intersected[0])
						mTarget->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
					mTarget = intersected[0];
					mTarget->getComponent<MeshRenderer>()->changeColor(SELECTED_OBJECT_COLOR);
					CalculateObjectPlane();
				}
				else
				{
					if (mTarget)
						mTarget->getComponent<MeshRenderer>()->changeColor(DEFAULT_OBJECT_COLOR);
					mTarget = nullptr;
				}


				if (mTarget)
					std::cout << "HIT: " << mTarget->getName() << '\n';
			}
		}
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS)
		{
			//std::cout << "SHOULD ROTATE CAMERA \n";
			this->canRotateCamera = true;
			glfwGetCursorPos(window, &this->mousePosX, &this->mousePosY);
			GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
			glfwSetCursor(window, cursor);
		}
		else
		{
			glfwSetCursor(window, NULL);
			this->canRotateCamera = false;
		}
		break;
	default:
		break;
	}
	if (mTarget && this->state != ADD)
	{
		this->state = NOTHING;
	}
}

void StateMachine::CalculateObjectPlane()
{
	this->objectPlane.normal = -camera->GetCameraForward();
	this->objectPlane.D = - glm::dot(this->objectPlane.normal, mTarget->getComponent<Transform>()->getPosition());
}

glm::vec3 StateMachine::CalculateIntersectionPoint()
{
	float t;

	t = (-glm::dot(this->objectPlane.normal, glm::vec3(this->mouseStartWorld)) - this->objectPlane.D) / glm::dot(this->objectPlane.normal, this->mouseDirectionWorld);
	return glm::vec3(this->mouseStartWorld) + this->mouseDirectionWorld * t;

}
void StateMachine::MouseMove(GLFWwindow* window, Camera& camera, const double mouseX, const double mouseY)
{
	/*if (mTarget && !this->canRotateCamera) return;

	else if (mTarget) camera.ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);*/

	if (mTarget) camera.ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);

	if (this->canRotateCamera)
	{
		this->camera->Rotate(window, this->mousePosX, this->mousePosY, mouseX, mouseY);
		this->mousePosX = mouseX;
		this->mousePosY = mouseY;
	}

	switch (this->state)
	{
	case GRAB:
		//std::cout << "SHOULD CHANGE TO GRAB" << '\n';
		Grab();
		break;
	case ROTATE:
		Rotate();
		break;
	case SCALE:
		Scale();
		break;
	default:
		break;
	}
}
void StateMachine::CheckTarget()
{
	//these states need to have target in order to be performed
	if (mTarget == nullptr && 
		(this->state == GRAB || this->state == ROTATE ||
		this->state == SCALE || this->state == DELETE))
	{
		this->state = NOTHING;
		this->followMouse = false;
	}
	else
	{
		this->followMouse = true;
	}
	return;
}

void StateMachine::Grab()
{
	glm::vec3 translationVector = CalculateIntersectionPoint();
	float xValue, yValue, zValue;
	glm::vec3 objectPos = mTarget->getComponent<Transform>()->getPosition();
	switch (this->subState)
	{
	case X: 
		xValue = (glm::vec3(1.0f, 0.0f, 0.0f) *
			glm::length(translationVector) * glm::normalize(translationVector)).x;
		translationVector = glm::vec3(xValue, objectPos.y, objectPos.z);
		break;
	case Y:
		yValue = (glm::vec3(0.0f, 1.0f, 0.0f) *
			glm::length(translationVector) * glm::normalize(translationVector)).y;
		translationVector = glm::vec3(objectPos.x, yValue, objectPos.z);
		break;
	case Z:
		zValue = (glm::vec3(0.0f, 0.0f, 1.0f) *
			glm::length(translationVector) * (translationVector.y < objectPos.y ? -1.0f : 1.0f)).z;
		translationVector = glm::vec3(objectPos.x, objectPos.y, zValue);
		break;

	default:
		break;
	}

	mTarget->getComponent<Transform>()->setPosition(translationVector);

}
void StateMachine::Rotate()
{
	//std::cout << "IN ROTATE " << std::endl;
}

void StateMachine::Scale()
{
	glm::vec3 translationVector = CalculateIntersectionPoint();
	float scalingFactor = glm::distance(translationVector, mTarget->getComponent<Transform>()->getPosition());
	mTarget->getComponent<Transform>()->setScale(scalingFactor);
}

void StateMachine::AddObject(Ray* ray)
{
	MeshLoader* meshLoaderObj;
	/*std::cout << "MESH LOADERS SIZE AT ADD: " << mMeshLoaders.size() << std::endl;
	std::cout << "SUBSTATE: " << subState << std::endl;*/
	if (this->subState != EMPTY && this->subState < mMeshLoaders.size())
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
	mObjectsInScene.push_back(std::move(obj));
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

