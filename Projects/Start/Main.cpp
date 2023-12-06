#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Lighting.h"

#define GLFW_HAND_CURSOR 0x00036004

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int globalWidth = 800, globalHeight= 800;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
double globalMouseXPos = globalWidth / 2;
double globalMouseYPos = globalHeight / 2;
bool canRotate = false;
GLFWcursor* cursor = nullptr;
Camera globalCamera(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 2.5f);


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
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	glViewport(0, 0, globalWidth, globalHeight);

	Shader shaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");

	Mesh cube("kocka.txt");
	Mesh dragon("dragon.txt");
	Mesh temple("tsd00.txt");
	Mesh bull("bull.txt");
	Mesh teddy("teddy.txt");

	Lighting light(glm::vec3(0.0f, 5.0f, 5.0f), cube);

	glm::mat4 lightModelMatrix = light.ModelMatrix();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShaderProgram.Activate();

		light.Draw(lightingShaderProgram, globalCamera);

		shaderProgram.Activate();

		dragon.Draw(shaderProgram, globalCamera, glm::vec3(0.0f, 0.0f, 0.0f));

		temple.Draw(shaderProgram, globalCamera, glm::vec3(1.0f, 0.0f, 0.0f));

		bull.Draw(shaderProgram, globalCamera, glm::vec3(0.0f, 1.0f, 0.0f));

		teddy.Draw(shaderProgram, globalCamera, glm::vec3(0.0f, -1.0f, 0.0f));
		globalCamera.Move(window, deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightingShaderProgram.Delete();
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
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS)
		{
			globalCamera.focus = !globalCamera.focus;
			if (!globalCamera.focus)
			{
				globalCamera.cameraDirection = glm::normalize(globalCamera.targetPos - globalCamera.cameraPos);
			}
			globalCamera.lookAtPosition = globalCamera.focus ? globalCamera.targetPos : (globalCamera.cameraPos + globalCamera.cameraDirection);
		}
		break;
	default:
		break;
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (canRotate)
	{
		globalCamera.Rotate(window, globalMouseXPos, globalMouseYPos, xpos, ypos);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		canRotate = true;
		glfwGetCursorPos(window, &globalMouseXPos, &globalMouseYPos);
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		glfwSetCursor(window, cursor);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		glfwSetCursor(window, NULL);
		canRotate = false;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwGetWindowSize(window, &globalWidth, &globalHeight);
	glViewport(0, 0, width, height);
} 