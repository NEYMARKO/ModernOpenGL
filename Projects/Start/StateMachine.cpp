#include "StateMachine.h"
#define GLFW_HAND_CURSOR 0x00036004

StateMachine::StateMachine(Mesh* mesh, Camera* camera, std::vector<MeshLoader*>& meshLoaders, std::vector<Mesh*>& objectsInScene) 
	: objectsInScene(objectsInScene), meshLoaders(meshLoaders)
{
	this->state = NOTHING;
	this->subState = EMPTY;
	this->target = mesh;
	this->camera = camera;
	this->mousePosX = this->camera->width / 2;
	this->mousePosY = this->camera->height / 2;
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
				this->target->ChangeColor(glm::vec3(0.862745f, 0.862745f, 0.862745f));
				this->target = nullptr;
				this->state = NOTHING;
				this->subState = EMPTY;
			}
			else
			{
				bool objectPicked = false;
				int pickedId = -1;
				for (int obj = 0; obj < this->objectsInScene.size() && !objectPicked; obj++)
				{
					for (float i = 0; i < ray->GetRayLength(); i += 0.25)
					{
						if (this->objectsInScene[obj]->boundingBox->Intersects(camera, i))
						{
							this->objectsInScene[obj]->ChangeColor(glm::vec3(0.0f, 1.0f, 0.0f));
							pickedId = this->objectsInScene[obj]->id;
							objectPicked = true;
							this->target = this->objectsInScene[obj];
							CalculateObjectPlane();
							break;
						}
					}
				}


				//if not a single object was clicked on, reset target to nullptr
				if (pickedId == -1) this->target = nullptr;
				//removing selective color if current click doesn't intersect with any of the objects
				for (int i = 0; i < this->objectsInScene.size(); i++)
				{
					if (this->objectsInScene[i]->id != pickedId) this->objectsInScene[i]->ChangeColor(glm::vec3(0.862745f, 0.862745f, 0.862745f));
				}
			}
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
	if (this->subState != EMPTY && this->subState < this->meshLoaders.size())
	{
		meshLoaderObj = this->meshLoaders[this->subState];
	}
	else
	{
		meshLoaderObj = this->meshLoaders[0];
	}
	Mesh* obj = new Mesh(meshLoaderObj, (ray->GetRayStart() + ray->GetRayDirection() * 10.0f), this->objectsInScene.size());
	this->objectsInScene.push_back(obj);
}
void StateMachine::DeleteObject()
{
	int targetPos = this->target->id;

	this->objectsInScene.erase(this->objectsInScene.begin() + targetPos);
	
	for (int i = targetPos; i < this->objectsInScene.size(); i++)
	{
		this->objectsInScene[i]->id--;
	}

	delete this->target;
	this->state = NOTHING;
}
void StateMachine::CloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<Mesh*>* StateMachine::GetObjectsInScene()
{
	return &this->objectsInScene;
}

bool StateMachine::ShouldFollowMouse()
{
	return this->followMouse;
}

StateMachine::~StateMachine()
{
	//deleting objects, it isn't enough to just clear the vector - objects have to be removed from heap
	//objects should be deleted from the back of the vector, because vector is getting smaller with each delete operation
	for (int i = this->objectsInScene.size() - 1; i >=0; i--)
	{
		delete this->objectsInScene[i];
	}
	std::cout << "Deleted state machine" << std::endl;
}