#include "Camera.h"
#include "Lighting.h"
#include "StateMachine.h"
#include "Grid.h"
#include "KinematicChain.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset);

int globalWidth = 800, globalHeight= 800;
float deltaTime = 0.0f, lastFrame = 0.0f;

Camera globalCamera(glm::vec3(0.0f, 0.0f, -7.5f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 1.0f, globalWidth, globalHeight);

std::vector<Mesh*> objectsInScene;
std::vector<MeshLoader*> meshLoaders;
StateMachine stateMachine(nullptr, &globalCamera, meshLoaders ,objectsInScene);


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
	Shader boundingBoxShaderProgram("borderBox.vert", "borderBox.frag");

	MeshLoader lightBulbLoader("lightBulb.txt");
	MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader dragonLoader("dragonSmooth.txt");
	MeshLoader templeLoader("templeFlat.txt");
	MeshLoader frogLoader("frogSmooth.txt");
	MeshLoader teddyLoader("teddyFlat.txt");
	MeshLoader sphereLoader("sphere.txt");
	//BLENDER: rotate around X for 270 (-90) degrees,	EXPORT: forward: -X, up: Z
	MeshLoader coneLoader("cone.txt");
	MeshLoader jointLoader("joint3.txt");

	meshLoaders.push_back(&cubeLoader);
	meshLoaders.push_back(&dragonLoader);
	meshLoaders.push_back(&templeLoader);
	meshLoaders.push_back(&frogLoader);
	meshLoaders.push_back(&teddyLoader);
	meshLoaders.push_back(&sphereLoader);
	meshLoaders.push_back(&coneLoader);
	meshLoaders.push_back(&jointLoader);

	float id = 0;

	Mesh* lightBulb = new Mesh(&lightBulbLoader, glm::vec3(-5.0f, 4.0f, 0.0f), id++);
	Mesh* dragon = new Mesh(&dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	//Mesh* cube = new Mesh(&cubeLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	Mesh* sphere = new Mesh(&sphereLoader, glm::vec3(-15.0f, 0.0f, 0.0f), id++);
	//Mesh* cone = new Mesh(&coneLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	Mesh* joint = new Mesh(&jointLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);

	objectsInScene.push_back(lightBulb);
	objectsInScene.push_back(dragon);
	//objectsInScene.push_back(cube);
	objectsInScene.push_back(sphere);
	//objectsInScene.push_back(cone);
	objectsInScene.push_back(joint);

	Lighting light(*lightBulb, glm::vec3(1.0f, 1.0f, 1.0f));

	Grid grid(100);

	KinematicChain ikChain(5, 90.0f, glm::vec3(0.0f, 0.0f, 0.0f), joint, sphere);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		light.Draw(lightingShaderProgram, boundingBoxShaderProgram, globalCamera);

		grid.Draw(boundingBoxShaderProgram, globalCamera);
		
		for (int i = 0; i < (*stateMachine.GetObjectsInScene()).size(); i++)
		{
			(*stateMachine.GetObjectsInScene())[i]->Render(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
			//(*stateMachine.GetObjectsInScene())[i]->boundingBox->Draw(boundingBoxShaderProgram, globalCamera);
		}
		
		if (globalCamera.ray != nullptr)
		{
			globalCamera.ray->Draw(boundingBoxShaderProgram, globalCamera);
		}

		ikChain.FabrikAlgorithm(10);

		//Move joint to it's new position and render it
		for (Joint* joint : (*ikChain.GetAllJoints()))
		{
			joint->GetMeshContainer()->Translate(joint->GetPosition());
			joint->GetMeshContainer()->Render(shaderProgram, boundingBoxShaderProgram, globalCamera, light);
		}
		globalCamera.Move(window, deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	stateMachine.ChangeState(window, key, action, globalCamera);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	stateMachine.MouseMove(window, globalCamera, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	stateMachine.MouseClick(window, globalCamera, button, action);
}

void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset)
{
	globalCamera.Zoom(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glfwGetWindowSize(window, &globalWidth, &globalHeight);
	globalCamera.UpdateViewportDimensions(globalWidth, globalHeight);
	glViewport(0, 0, globalWidth, globalHeight);
} 