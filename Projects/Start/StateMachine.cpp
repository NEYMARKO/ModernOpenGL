#include "StateMachine.h"

StateMachine::StateMachine(Mesh* mesh, Camera* camera)
{
	this->state = NOTHING;
	this->target = mesh;
	this->camera = camera;
	
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
			break;
		}
	CheckTarget();
	ControlState(window);
	}
	//std::cout << "STATE: " << this->state << std::endl;
}

void StateMachine::Click(GLFWwindow* window, Camera& camera, std::vector<Mesh*>& objectsInScene, MeshLoader* meshLoaderObj, int button, int action)
{
	this->_objectsInScene = &objectsInScene;
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
				Mesh* obj = new Mesh(meshLoaderObj, (ray->GetRayStart() + ray->GetRayDirection() * 10.0f), objectsInScene.size());
				objectsInScene.push_back(obj);
				//std::cout << "SIZE AFTER ADD: " << objectsInScene.size() << std::endl;
			}
			else if (this->state == GRAB || this->state == SCALE || this->state == ROTATE)
			{
				this->target->ChangeColor(glm::vec3(1.0f, 0.5f, 0.31f));
				this->target = nullptr;
				this->state = NOTHING;
			}
			else
			{
				bool objectPicked = false;
				int pickedId = -1;
				for (int obj = 0; obj < objectsInScene.size() && !objectPicked; obj++)
				{
					//std::cout << "CHECKING FOR ID: " << objectsInScene[obj]->id << std::endl;
					for (float i = 0; i < ray->GetRayLength(); i += 0.25)
					{
						if (objectsInScene[obj]->boundingBox->Intersects(camera, i))
						{
							//std::cout << "HIT WITH: " << objectsInScene[obj]->id << std::endl;
							objectsInScene[obj]->ChangeColor(glm::vec3(0.0f, 1.0f, 0.0f));
							pickedId = objectsInScene[obj]->id;
							objectPicked = true;
							this->target = objectsInScene[obj];
							CalculateObjectPlane();
							break;
						}
					}
				}


				//if not a single object was clicked on, reset target to nullpointer
				if (pickedId == -1) this->target = nullptr;
				//removing selective color if current click doesn't intersect with any of the objects
				for (int i = 0; i < objectsInScene.size(); i++)
				{
					if (objectsInScene[i]->id != pickedId) objectsInScene[i]->ChangeColor(glm::vec3(1.0f, 0.5f, 0.31f));
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
	std::cout << "T: " << t << std::endl;
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
	/*for (int i = 0; i < this->_objectsInScene->size(); i++)
	{
		std::cout << "CURRENT IDS: " << (*this->_objectsInScene)[i]->id << std::endl;
	}*/
	short targetPos = this->target->id;
	/*std::cout << "ID OF OBJECT TO DELETE: " << targetPos << std::endl;
	std::cout << "VECTOR SIZE: " << (*this->_objectsInScene).size() << std::endl;*/
	(*this->_objectsInScene).erase((*this->_objectsInScene).begin() + targetPos);
	
	for (int i = targetPos; i < (*this->_objectsInScene).size(); i++)
	{
		(*this->_objectsInScene)[i]->id--;
	}
	this->state = NOTHING;
	//std::cout << "IN DELETE " << std::endl;
}
void StateMachine::CloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}
StateMachine::~StateMachine()
{
	std::cout << "Deleted state machine" << std::endl;
}