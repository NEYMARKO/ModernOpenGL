#include "Camera.h"
#include "Lighting.h"
#include "StateMachine.h"
#define GLFW_HAND_CURSOR 0x00036004

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset);

int globalWidth = 800, globalHeight= 800;
float deltaTime = 0.0f, lastFrame = 0.0f;
double globalMouseXPos = globalWidth / 2, globalMouseYPos = globalHeight / 2;
bool canRotate = false;
//bool canDrawRay = false;
//bool canAdd = false;

float id = 0;


GLFWcursor* cursor = nullptr;
Camera globalCamera(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.5f, 50.0f, globalWidth, globalHeight);
Shader boundingBoxShaderProgram;

std::vector<Mesh*> objectsInScene;
MeshLoader cubeLoader("cubeFlat.txt");
//MeshLoader dragonLoader("dragonSmooth.txt");
StateMachine stateMachine(nullptr, &globalCamera);


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Intro to Modern OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_back);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	glViewport(0, 0, globalWidth, globalHeight);

	Shader shaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");
	boundingBoxShaderProgram = Shader("borderBox.vert", "borderBox.frag");

	MeshLoader lightBulbLoader("lightBulb.txt");
	MeshLoader dragonLoader("dragonSmooth.txt");
	MeshLoader planeLoader("planeFlat.txt");
	/*MeshLoader templeLoader("templeFlat.txt");
	MeshLoader frogLoader("frogSmooth.txt");
	MeshLoader teddyLoader("teddyFlat.txt");*/

	Mesh lightBulb(&lightBulbLoader, glm::vec3(-5.0f, 4.0f, 0.0f), id++);
	//Mesh dragon(&dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	Mesh cube(&cubeLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	Mesh plane(&planeLoader, glm::vec3(0.0f, -4.0f, 0.0f), id++);
	/*Mesh dragon(&dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	Mesh dragon2(&dragonLoader, glm::vec3(5.0f + 2.0f, 4.0f - 3.0f, 0.0f - 3.0f), id++);
	Mesh dragon3(&dragonLoader, glm::vec3(5.0f - 3.0f, 4.0f + 2.0f, 0.0f), id++);
	Mesh dragon4(&dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	Mesh dragon5(&dragonLoader, glm::vec3(5.0f - 3.0f, 4.0f, 0.0f + 2.0f), id++);
	Mesh dragon6(&dragonLoader, glm::vec3(5.0f, 4.0f - 3.0f, 0.0f), id++);*/
	/*Mesh temple(&templeLoader, glm::vec3(-3.0f, 2.0f, 0.0f), id++);
	Mesh frog(&frogLoader, glm::vec3(0.0f, -4.0f, 0.0f), id++);
	Mesh teddy(&teddyLoader, glm::vec3(-2.0f, 4.0f, 0.0f), id++);*/


	
	objectsInScene.push_back(&cube);
	objectsInScene.push_back(&plane);
	objectsInScene.push_back(&lightBulb);
	//objectsInScene.push_back(&dragon);
	/*objectsInScene.push_back(&cube);
	objectsInScene.push_back(&dragon);
	objectsInScene.push_back(&dragon2);
	objectsInScene.push_back(&dragon3);
	objectsInScene.push_back(&dragon4);
	objectsInScene.push_back(&dragon5);
	objectsInScene.push_back(&dragon6);*/
	/*objectsInScene.push_back(&teddy);
	objectsInScene.push_back(&temple);
	objectsInScene.push_back(&frog);*/

	Lighting light(lightBulb, glm::vec3(1.0f, 1.0f, 1.0f));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		light.Draw(lightingShaderProgram, boundingBoxShaderProgram, globalCamera);

		/*dragon.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		dragon2.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		dragon3.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		dragon4.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		dragon5.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		dragon6.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);*/

		//std::cout << "OBJECTS IN SCENE SIZE: " << objectsInScene.size() << std::endl;
		for (int i = 0; i < objectsInScene.size(); i++)
		{
			//std::cout << "OBJECT IN SCENE ID: " << objectsInScene[i]->id << std::endl;
			objectsInScene[i]->Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		}
		//dragonRotated.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);

		/*temple.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		
		frog.Draw(shaderProgram, boundingBoxShaderProgram,  globalCamera, light);
		
		teddy.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);*/

		//cube.Draw(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		
		if (globalCamera.ray != nullptr)
		{
			globalCamera.ray->Draw(boundingBoxShaderProgram, globalCamera);
		}

		globalCamera.Move(window, deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	objectsInScene.clear();

	shaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	//case GLFW_KEY_ESCAPE:
	//	glfwSetWindowShouldClose(window, GL_TRUE);
	//	break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS)
		{
			globalCamera.focus = !globalCamera.focus;
			if (!globalCamera.focus)
			{
				//reset camera direction to face target (it wasn't getting updated while focus was on)
				globalCamera.cameraDirection = glm::normalize(globalCamera.targetPos - globalCamera.cameraPos);
			}
			globalCamera.lookAtPosition = globalCamera.focus ? globalCamera.targetPos : (globalCamera.cameraPos + globalCamera.cameraDirection);
		}
		break;
	//case GLFW_KEY_A:
	//	if (action == GLFW_PRESS)
	//	{
	//		canAdd = !canAdd;
	//	}
	//	break;
	//default:
	//	break;
	}
	stateMachine.ChangeState(window, key, action, globalCamera);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "SHOULD FOLLOW MOUSE: " << (stateMachine.ShouldFollowMouse() ? "true" : "false") << std::endl;
	if (stateMachine.ShouldFollowMouse())
	{
		//std::cout << "MOUSE POSITION CALLBACK " << std::endl;
		stateMachine.MouseMove(window, globalCamera, xpos, ypos);
	}
	if (canRotate)
	{
		globalCamera.Rotate(window, globalMouseXPos, globalMouseYPos, xpos, ypos);
		globalMouseXPos = xpos;
		globalMouseYPos = ypos;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_PRESS)
			{
				canRotate = true;
				glfwGetCursorPos(window, &globalMouseXPos, &globalMouseYPos);
				GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
				glfwSetCursor(window, cursor);
			}
			else
			{
				glfwSetCursor(window, NULL);
				canRotate = false;
			}
			break;
		default:
			break;
	}
	stateMachine.Click(window, globalCamera, objectsInScene, &cubeLoader, button, action);
}

void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "ZOOM: " << yoffset << std::endl;
	globalCamera.Zoom(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwGetWindowSize(window, &globalWidth, &globalHeight);
	globalCamera.UpdateViewportDimensions(globalWidth, globalHeight);
	glViewport(0, 0, globalWidth, globalHeight);
} 