#include "StateMachine.h"

StateMachine::StateMachine(Mesh* mesh, Camera* camera, std::vector<Mesh*>& objectsInScene) : objectsInScene(objectsInScene)
{
	this->state = NOTHING;
	this->subState = EMPTY;
	this->target = mesh;
	this->camera = camera;
	//this->objectsInScene = objectsInScene;
	//std::cout << "State machine created" << std::endl;
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
			//if adding is done, state for adding meshloaders should be empty
			if (this->state == NOTHING) this->subState = EMPTY;
			break;
		case GLFW_KEY_DELETE:
			this->state = DELETE;
			break;
		case GLFW_KEY_F:
			this->state = FOCUS;
			break;
		case GLFW_KEY_ESCAPE:
			this->state = CLOSE_WINDOW;
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
		//while moving camera perserve same state
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
	CheckTarget();
	ControlState(window);
	}
	//std::cout << "STATE: " << this->state << std::endl;
}

void StateMachine::Click(GLFWwindow* window, Camera& camera, std::vector<MeshLoader*>& meshLoaders, int button, int action)
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

			//std::cout << "BEFORE IF" << std::endl;
			if (this->state == ADD)
			{
				//std::cout << "IN IF" << std::endl;
				//std::cout << "SIZE BEFORE ADD: " << objectsInScene.size() << std::endl;
				MeshLoader* meshLoaderObj;
				if (this->subState != EMPTY && this->subState < meshLoaders.size())
				{
					meshLoaderObj = meshLoaders[this->subState];
				}
				else
				{
					meshLoaderObj = meshLoaders[0];
				}
				Mesh* obj = new Mesh(meshLoaderObj, (ray->GetRayStart() + ray->GetRayDirection() * 10.0f), this->objectsInScene.size());
				this->objectsInScene.push_back(obj);
				//std::cout << "SIZE AFTER ADD: " << objectsInScene.size() << std::endl;
			}
			//object transformation has been completed
			else if (this->state == GRAB || this->state == SCALE || this->state == ROTATE)
			{
				this->target->ChangeColor(glm::vec3(1.0f, 0.5f, 0.31f));
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
					//std::cout << "CHECKING FOR ID: " << objectsInScene[obj]->id << std::endl;
					for (float i = 0; i < ray->GetRayLength(); i += 0.25)
					{
						if (this->objectsInScene[obj]->boundingBox->Intersects(camera, i))
						{
							//std::cout << "HIT WITH: " << objectsInScene[obj]->id << std::endl;
							this->objectsInScene[obj]->ChangeColor(glm::vec3(0.0f, 1.0f, 0.0f));
							pickedId = this->objectsInScene[obj]->id;
							objectPicked = true;
							this->target = this->objectsInScene[obj];
							CalculateObjectPlane();
							break;
						}
					}
				}


				//if not a single object was clicked on, reset target to nullpointer
				if (pickedId == -1) this->target = nullptr;
				//removing selective color if current click doesn't intersect with any of the objects
				for (int i = 0; i < this->objectsInScene.size(); i++)
				{
					if (this->objectsInScene[i]->id != pickedId) this->objectsInScene[i]->ChangeColor(glm::vec3(1.0f, 0.5f, 0.31f));
				}
			}
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

	this->objectPlane.normal = -camera->cameraDirection;
	this->objectPlane.D = - glm::dot(this->objectPlane.normal, this->target->objectPos);
	
	/*std::cout << "CAMERA DIRECTION: " << this->camera->cameraDirection.x << ", " << this->camera->cameraDirection.y
		<< ", " << this->camera->cameraDirection.z << std::endl;
	std::cout << "PLANE NORMAL: " << this->objectPlane.normal.x << ", " << this->objectPlane.normal.y
		<< ", " << this->objectPlane.normal.z << std::endl;*/
	/*std::cout << "CAMERA POSITION: " << this->camera->cameraPos.x << ", " << this->camera->cameraPos.y
		<< ", " << this->camera->cameraPos.z << std::endl;*/
	//std::cout << "D: " << this->objectPlane.D << std::endl;
}

glm::vec3 StateMachine::CalculateIntersectionPoint()
{
	float t;

	t = (-glm::dot(this->objectPlane.normal, glm::vec3(this->mouseStartWorld)) - this->objectPlane.D) / glm::dot(this->objectPlane.normal, this->mouseDirectionWorld);
	//std::cout << "T: " << t << std::endl;
	return glm::vec3(this->mouseStartWorld) + this->mouseDirectionWorld * t;
}
void StateMachine::MouseMove(GLFWwindow* window, Camera& camera, const double mouseX, const double mouseY)
{
	//glm::vec4 mousePositionWorld = glm::vec4(1.0);
	//std::cout << "MOUSE MOVE IN STATE MACHINE " << std::endl;
	if (!this->target) return;
	//CalculateObjectPlane();

	camera.ScreenToWorldCoordinates(mouseX, mouseY, this->mouseStartWorld, this->mouseDirectionWorld);
	//this->mousePositionWorld.z = this->mousePositionWorld.z / this->target->boundingBox->boxCenter.z;
	//this->mouseEndWorld.z = this->target->objectPos.z;
	/*std::cout << "Mouse position in world " << this->mousePositionWorld.x << ", " 
		<< this->mousePositionWorld.y << ", " << this->mousePositionWorld.z << std::endl;*/
	switch (this->state)
	{
	case GRAB:
		this->followMouse = true;
		Grab();
		break;
	case ROTATE:
		this->followMouse = true;
		Rotate();
		break;
	case SCALE:
		this->followMouse = true;
		Scale();
		break;
	case ADD:
		Add();
		break;
	case DELETE:
		//Delete();
		break;
	case CLOSE_WINDOW:
		CloseWindow(window);
		break;
	default:
		this->followMouse = false;
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
	}
	return;
}

bool StateMachine::ShouldFollowMouse()
{
	return this->followMouse;
}

void StateMachine::ControlState(GLFWwindow* window)
{
	switch (this->state)
	{
	case GRAB:
		this->followMouse = true;
		break;
	case ROTATE:
		this->followMouse = true;
		break;
	case SCALE:
		this->followMouse = true;
		break;
	case ADD:
		break;
	case DELETE:
		Delete();
		break;
	case CLOSE_WINDOW:
		CloseWindow(window);
		break;
	default:
		this->followMouse = false;
		break;
	}
}

void StateMachine::Grab()
{
	//std::cout << "IN GRAB " << std::endl;
	//glm::vec3 translationVector = glm::vec3(this->mousePositionWorld) - this->target->objectPos;

	//std::cout << std::endl << "STARTING VECTOR: " << this->mouseStartWorld.x << ", " << this->mouseStartWorld.y << ", " << this->mouseStartWorld.z << std::endl;
	/*std::cout << "CAMERA DIRECTION: " << this->camera->cameraDirection.x << ", " << this->camera->cameraDirection.y
		<< ", " << this->camera->cameraDirection.z << std::endl;*/
	
	//float turnFactor = this->objectPlane.D > this->camera->cameraPos.z ? 1.0f : -1.0f;
	
	/*std::cout << "CAMERA POSITION: " << this->camera->cameraPos.x << ", " << this->camera->cameraPos.y
		<< ", " << this->camera->cameraPos.z << std::endl;
	std::cout << "D: " << this->objectPlane.D << std::endl;*/
	/*glm::vec3 translationVector = glm::vec3(this->mouseStartWorld) +
		this->mouseDirectionWorld * glm::distance(this->objectPlane.normal, this->camera->cameraPos) * this->camera->cameraDirection.z;*/
	glm::vec3 translationVector = CalculateIntersectionPoint();

	
	/*glm::vec3 translationVector = glm::vec3(this->mouseStartWorld) +
		glm::normalize(glm::vec3(this->mouseEndWorld) - glm::vec3(this->mouseStartWorld)) * (this->camera->cameraPos.z - this->objectPlane.D) * this->camera->cameraDirection.z;*/

	/*std::cout << "Camera position" << this->camera->cameraPos.x << ", "
		<< this->camera->cameraPos.y << ", " << this->camera->cameraPos.z << std::endl;*/
	//translationVector *= 5;
	//this->target->Translate();
	
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



	/*std::cout << "Mouse position in world: " << this->mouseEndWorld.x << ", "
		<< this->mouseEndWorld.y << ", " << this->mouseEndWorld.z << std::endl;*/
	/*std::cout << "Translate position in world " << translationVector.x << ", "
		<< translationVector.y << ", " << translationVector.z << std::endl;*/
	/*std::cout << "Translation vector: " << translationVector.x << ", "
		<< translationVector.y << ", " << translationVector.z << std::endl;*/
	this->target->Translate(translationVector);
	//this->target->Translate(glm::vec3(0.0, 0.0, 0.0));

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
	//glm::vec3 scalingAmount = glm::length(mousePositionVector - this->target->boundingBox.center)
	//std::cout << "IN SCALE " << std::endl;
}
void StateMachine::Add()
{
	//std::cout << "IN ADD " << std::endl;
}
void StateMachine::Delete()
{
	//std::cout << "IN DELETE " << std::endl;
	//for (int i = 0; i < this->objectsInScene.size(); i++)
	//{
	//	std::cout << "CURRENT IDS: " << this->objectsInScene[i]->id << std::endl;
	//}
	short targetPos = this->target->id;
	/*std::cout << "ID OF OBJECT TO DELETE: " << targetPos << std::endl;
	std::cout << "VECTOR SIZE: " << this->objectsInScene.size() << std::endl;*/
	this->objectsInScene.erase(this->objectsInScene.begin() + targetPos);
	
	for (int i = targetPos; i < this->objectsInScene.size(); i++)
	{
		this->objectsInScene[i]->id--;
	}

	//this->target = nullptr;
	delete this->target;
	this->state = NOTHING;
	//std::cout << "IN DELETE " << std::endl;
}
void StateMachine::CloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}
StateMachine::~StateMachine()
{
	for (int i = this->objectsInScene.size() - 1; i >=0; i--)
	{
		delete this->objectsInScene[i];
	}
	std::cout << "Deleted state machine" << std::endl;
}