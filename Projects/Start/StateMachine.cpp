#include "StateMachine.h"

StateMachine::StateMachine(Mesh* mesh)
{
	this->state = NOTHING;
	this->target = mesh;
	std::cout << "State machine created" << std::endl;
}

void StateMachine::ChangeState(GLFWwindow* window, const int key, const int action)
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
		default:
			this->state = NOTHING;
			break;
		}
	}
	std::cout << "STATE: " << this->state << std::endl;
	CheckTarget();
	ControlState(window);
}

void StateMachine::Click(GLFWwindow* window, Camera& camera, std::vector<Mesh*>& objectsInScene, MeshLoader* meshLoaderObj, int button, int action)
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
				//std::cout << "IN HERE" << std::endl;
				Mesh* obj = new Mesh(meshLoaderObj, (ray->GetRayStart() + ray->GetRayDirection() * 10.0f), objectsInScene.size() + 1);
				objectsInScene.push_back(obj);
			}
			else
			{
				bool objectPicked = false;
				int pickedId = -1;
				for (int obj = 0; obj < objectsInScene.size() && !objectPicked; obj++)
				{
					for (float i = 0; i < ray->GetRayLength(); i += 0.5)
					{
						if (objectsInScene[obj]->boundingBox->Intersects(camera, i))
						{
							objectsInScene[obj]->ChangeColor(glm::vec3(0.0f, 1.0f, 0.0f));
							pickedId = objectsInScene[obj]->id;
							objectPicked = true;
							this->target = objectsInScene[obj];
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

void StateMachine::ControlState(GLFWwindow* window)
{
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
	case ADD:
		Add();
		break;
	case DELETE:
		Delete();
		break;
	case CLOSE_WINDOW:
		CloseWindow(window);
		break;
	default:
		break;
	}
}

void StateMachine::Grab()
{
	std::cout << "IN GRAB " << std::endl;
}
void StateMachine::Rotate()
{
	std::cout << "IN ROTATE " << std::endl;
}
void StateMachine::Scale()
{
	std::cout << "IN SCALE " << std::endl;
}
void StateMachine::Add()
{
	std::cout << "IN ADD " << std::endl;
}
void StateMachine::Delete()
{
	std::cout << "IN DELETE " << std::endl;
}
void StateMachine::CloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}
StateMachine::~StateMachine()
{
	std::cout << "Deleted state machine" << std::endl;
}