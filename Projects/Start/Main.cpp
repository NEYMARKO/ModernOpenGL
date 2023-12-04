#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Mesh.h"
#include "Lighting.h"

#define GLFW_HAND_CURSOR 0x00036004

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int globalWidth = 800, globalHeight= 800;

//std::vector<Vertex> vertices =
//{
//	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
//	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f)},
//	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 1.0f)},
//	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
//	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f)},
//	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
//	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
//	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)}
//};

std::vector<Vertex> vertices =
{
	{glm::vec3(0.5f, -0.5f, -0.5f) },
	{glm::vec3(0.5f, 0.5f, -0.5f) },
	{glm::vec3(-0.5f, 0.5f, -0.5f) },
	{glm::vec3(-0.5f, -0.5f, -0.5f) },
	{glm::vec3(0.5f, -0.5f, 0.5f) },
	{glm::vec3(0.5f, 0.5f, 0.5f) },
	{glm::vec3(-0.5f, 0.5f, 0.5f) },
	{glm::vec3(-0.5f, -0.5f, 0.5f)}
};


std::vector<unsigned int> indices =
{
	0, 1, 2,  // Front face
	2, 3, 0,
	4, 5, 6,  // Back face
	6, 7, 4,
	0, 3, 7,  // Top face
	7, 4, 0,
	1, 2, 6,  // Bottom face
	6, 5, 1,
	2, 6, 7,  // Right face
	7, 3, 2,
	0, 4, 5,  // Left face
	5, 1, 0
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

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

	Mesh cube(vertices, indices);
	Mesh all("arena.txt");
	Lighting light(glm::vec3(0.0f, 5.0f, 5.0f), cube);

	glm::mat4 lightModelMatrix = light.ModelMatrix();

	//Texture texture("container.jpg");
	//GLuint tex0Location = glGetUniformLocation(shaderProgram.ID, "tex0");
	//shaderProgram.Activate();
	//glUniform1i(tex0Location, 0);


	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		lightingShaderProgram.Activate();

		//light.BindVAO();
		globalCamera.ViewProjectionMatrix(globalCamera.lookAtPosition, shaderProgram, lightingShaderProgram);
		globalCamera.Move(window, deltaTime);
		
		unsigned int lightModelMatrixLocation = glGetUniformLocation(lightingShaderProgram.ID, "model");
		glUniformMatrix4fv(lightModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightModelMatrix));
		//glUniform1f(tex0Location, 0.5f);
		//texture.Bind();
		
		//light.Draw();

		light.Draw();
		/*cube.meshVAO.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		cube.meshVAO.Unbind();*/
		//light.UnbindVAO();

		shaderProgram.Activate();

		//OVO TREBA RADIT PRIJE CRTANJA SVAKOG OBJEKTA JER SE VIEW I PROJECTION MATRIX NE RETURNAJU UZ POMOC FUNKCIJE
		globalCamera.ViewProjectionMatrix(globalCamera.lookAtPosition, shaderProgram, lightingShaderProgram);
		globalCamera.Move(window, deltaTime);

		model = glm::scale(model, glm::vec3(all.scalingFactor, all.scalingFactor, all.scalingFactor));
		unsigned int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
		unsigned int objectColorLocation = glGetUniformLocation(shaderProgram.ID, "objectColor");
		unsigned int lightColorLocation = glGetUniformLocation(shaderProgram.ID, "lightColor");
		glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(objectColorLocation, 1, glm::value_ptr(objectColor));
		glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		all.Draw(shaderProgram);
		/*cube.mVAO.Bind();
		globalCamera.ViewProjectionMatrix(globalCamera.lookAtPosition, shaderProgram, lightingShaderProgram);
		globalCamera.Move(window, deltaTime);
		for (unsigned int i = 1; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));
			unsigned int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
			unsigned int objectColorLocation = glGetUniformLocation(shaderProgram.ID, "objectColor");
			unsigned int lightColorLocation = glGetUniformLocation(shaderProgram.ID, "lightColor");
			glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
			glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(objectColorLocation, 1, glm::value_ptr(objectColor));
			glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}

		cube.mVAO.Unbind();*/
		//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//texture.Delete();
	shaderProgram.Delete();
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