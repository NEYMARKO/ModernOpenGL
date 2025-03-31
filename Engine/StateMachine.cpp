#include "StateMachine.h"
#define GLFW_HAND_CURSOR 0x00036004
#define DEFAULT_OBJECT_COLOR glm::vec3(0.862745f, 0.862745f, 0.862745f)
#define SELECTED_OBJECT_COLOR glm::vec3(0.0f, 1.0f, 0.0f)

StateMachine::StateMachine(Mesh* mesh, Camera* camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Mesh>>& objectsInScene)
	: mObjectsInScene{ objectsInScene }, mMeshLoaders{ meshLoaders }
{
	this->state = NOTHING;
	this->subState = EMPTY;
	this->target = mesh;
	this->camera = camera;
	this->mousePosX = this->camera->width / 2;
	this->mousePosY = this->camera->height / 2;
	std::cout << "MESH LOADERS IN CONSTRUCTOR: " << std::endl;
	std::cout << meshLoaders.size() << std::endl;
	for (int i = 0; i < meshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER " << i << " VERTICES: " << meshLoaders[i]->vertices.size() << std::endl;
	}
}

void StateMachine::AddShaderPrograms(Shader* shader, Shader* boxShader)
{
	mShaderProgram = shader;
	mBoundingBoxShaderProgram = boxShader;
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
			if (this->target) DeleteObject();
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
			Ray* ray = camera.ray;

			if (this->state == ADD)
			{
				AddObject(ray);
			}
			//object transformation has been completed
			else if (this->state == GRAB || this->state == SCALE || this->state == ROTATE)
			{
				this->target->ChangeColor(DEFAULT_OBJECT_COLOR);
				this->target = nullptr;
				this->state = NOTHING;
				this->subState = EMPTY;
			}
			else
			{
				bool objectPicked = false;
				int pickedId = -1;
				//SortObjectsInScene();
				std::cout << "OBJ IN SCENE SIZE: " << mObjectsInScene.size() << std::endl;
				for (int obj = 0; obj < this->mObjectsInScene.size() && !objectPicked; obj++)
				{
					std::cout << "INSIDE OF OBJECTS IN SCENE " << std::endl;
					for (float i = 0; i < ray->GetRayLength(); i += 0.25)
					{
						if (mObjectsInScene[obj]->boundingBox->Intersects(camera, i))
						{
							this->mObjectsInScene[obj]->ChangeColor(SELECTED_OBJECT_COLOR);
							pickedId = this->mObjectsInScene[obj]->id;
							objectPicked = true;
							this->target = mObjectsInScene[obj].get();
							CalculateObjectPlane();
							break;
						}
					}
				}


				//if not a single object was clicked on, reset target to nullptr
				if (pickedId == -1) this->target = nullptr;
				//removing selective color if current click doesn't intersect with any of the objects
				for (int i = 0; i < this->mObjectsInScene.size(); i++)
				{
					if (this->mObjectsInScene[i]->id != pickedId) this->mObjectsInScene[i]->ChangeColor(DEFAULT_OBJECT_COLOR);
				}
			}
			std::cout << "AFTER ADD" << std::endl;
		}
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS)
		{
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
	if (!this->target && this->state != ADD)
	{
		this->state = NOTHING;
	}
}

void StateMachine::CalculateObjectPlane()
{
	this->objectPlane.normal = -camera->GetCameraForward();
	this->objectPlane.D = - glm::dot(this->objectPlane.normal, this->target->objectPos);
}

glm::vec3 StateMachine::CalculateIntersectionPoint()
{
	float t;

	t = (-glm::dot(this->objectPlane.normal, glm::vec3(this->mouseStartWorld)) - this->objectPlane.D) / glm::dot(this->objectPlane.normal, this->mouseDirectionWorld);
	return glm::vec3(this->mouseStartWorld) + this->mouseDirectionWorld * t;

}
void StateMachine::MouseMove(GLFWwindow* window, Camera& camera, const double mouseX, const double mouseY)
{
	if (!this->target && !this->canRotateCamera) return;

	else if (this->target) camera.ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);

	else if (this->canRotateCamera)
	{
		this->camera->Rotate(window, this->mousePosX, this->mousePosY, mouseX, mouseY);
		this->mousePosX = mouseX;
		this->mousePosY = mouseY;
	}

	switch (this->state)
	{
	case GRAB:
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
	if (this->target == nullptr && 
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

	switch (this->subState)
	{
	case X:
		xValue = (glm::vec3(1.0f, 0.0f, 0.0f) *
			glm::length(translationVector) * glm::normalize(translationVector)).x;
		translationVector = glm::vec3(xValue, this->target->objectPos.y, this->target->objectPos.z);
		break;
	case Y:
		yValue = (glm::vec3(0.0f, 1.0f, 0.0f) *
			glm::length(translationVector) * glm::normalize(translationVector)).y;
		translationVector = glm::vec3(this->target->objectPos.x, yValue, this->target->objectPos.z);
		break;
	case Z:
		zValue = (glm::vec3(0.0f, 0.0f, 1.0f) *
			glm::length(translationVector) * (translationVector.y < this->target->objectPos.y ? -1.0f : 1.0f)).z;
		translationVector = glm::vec3(this->target->objectPos.x, this->target->objectPos.y, zValue);
		break;

	default:
		break;
	}

	this->target->Translate(translationVector);

}
void StateMachine::Rotate()
{
	//std::cout << "IN ROTATE " << std::endl;
}

void StateMachine::Scale()
{
	glm::vec3 translationVector = CalculateIntersectionPoint();
	float scalingFactor = glm::distance(translationVector, this->target->objectPos);
	this->target->Scale(scalingFactor);
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
	auto obj = std::make_unique<Mesh>(mShaderProgram, mBoundingBoxShaderProgram, meshLoaderObj, (ray->GetRayStart() + ray->GetRayDirection() * 10.0f), this->mObjectsInScene.size());
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

//std::vector<std::unique_ptr<Mesh>> StateMachine::GetObjectsInScene()
//{
//	return mObjectsInScene;
//}

bool StateMachine::ShouldFollowMouse()
{
	return this->followMouse;
}

void StateMachine::SortObjectsInScene()
{
	/*for (Mesh* mesh : this->mObjectsInScene)
	{
		mesh->CalculateDistanceFromCamera(this->camera);
	}
	QuickSort(0, this->mObjectsInScene.size() - 1);*/
}

void StateMachine::QuickSort(const int& low, const int& high)
{
	if (low < high) {
		int pi = Partition(low, high);
		QuickSort(low, pi - 1);
		QuickSort(pi + 1, high);
	}
}

int StateMachine::Partition(const int& low, const int& high)
{
	float pivot = this->mObjectsInScene[high]->GetDistanceFromCamera();
	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (this->mObjectsInScene[j]->GetDistanceFromCamera() < pivot) {
			i++;
			Swap(i, j);
		}
	}
	Swap(i + 1, high);
	return i + 1;
}

void StateMachine::Swap(const int& firstPos, const int& secondPos)
{
	/*Mesh* temp = this->mObjectsInScene[firstPos];
	this->mObjectsInScene[firstPos] = this->mObjectsInScene[secondPos];
	this->mObjectsInScene[secondPos] = temp;*/
}
