#include "Window.h"
#include "Camera.h"
#include "StateMachine.h"

Window::Window(Camera* camera, int width = 1920, int height = 1080)
	: mWidth{ width }, mHeight{ height }, mCamera{ camera }
{
	initialize();
}

Window::~Window()
{

	//IN CASE OF having multiple Window instances, multiple calls to 
	// glfwTerminate should be prevented
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Window::initialize()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "Intro to Modern OpenGL", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		mStatus = INVALID;
		return;
	}

	glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, this);

	//CALLBACK FUNCTIONS
	glfwSetKeyCallback(mWindow, Window::key_callback);
	glfwSetCursorPosCallback(mWindow, Window::cursor_position_callback);
	glfwSetMouseButtonCallback(mWindow, Window::mouse_button_callback);
	glfwSetScrollCallback(mWindow, Window::mouse_scroll_back);
	glfwSetFramebufferSizeCallback(mWindow, Window::framebuffer_size_callback);

	//LOAD GLAD so it configures OpenGL
	gladLoadGL();
	glViewport(0, 0, mWidth, mHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mStatus = VALID;
}

void Window::addStateMachine(StateMachine* stateMachine)
{
	mStateMachine = stateMachine;
}

bool Window::loaded()
{
	return mStatus == VALID;
}

GLFWwindow* Window::getGLFWwindow()
{
	return mWindow;
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win && win->mStateMachine) win->mStateMachine->ChangeState(window, key, action, *(win->mCamera));
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win && win->mStateMachine) win->mStateMachine->MouseMove(window, *(win->mCamera), xpos, ypos);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win && win->mStateMachine) win->mStateMachine->MouseClick(window, *(win->mCamera), button, action);
}

void Window::mouse_scroll_back(GLFWwindow* window, double xOffset, double yOffset)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win && win->mCamera) win->mCamera->Zoom(yOffset);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win)
	{
		glfwGetWindowSize(window, &win->mWidth, &win->mHeight);
		win->mCamera->UpdateViewportDimensions(win->mWidth, win->mHeight);
		glViewport(0, 0, win->mWidth, win->mHeight);
	}
}