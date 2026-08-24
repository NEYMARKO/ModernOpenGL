#pragma once
#include <iostream>
#include <string_view>
//#include "OpenGLIncludes.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

//#include "Camera.h"
//#include "StateMachine.h"
class Camera;
class StateMachine;

enum Status
{
	WIN_NONE,
	WIN_VALID,
	WIN_INVALID
};

enum CursorShape
{
	DEFAULT_CURSOR,
	CLOSED_HAND_CURSOR
};

class Window
{
private:
	int mWidth;
	int mHeight;
	std::string_view m_name;
	Status mStatus {WIN_NONE};
	CursorShape m_cursorShape{ DEFAULT_CURSOR };
	GLFWwindow* mWindow;
	GLFWcursor* m_cursor;
	Camera* mCamera;
	StateMachine* mStateMachine;
public:
	Window(Camera* camera, int width, int height);
	~Window();
	//initializes glfw and loads glad 
	void initialize();
	void addStateMachine(StateMachine* stateMachine);
	void alignCameraToWindow();
	//returns true if window was successfully loaded, otherwise returns false
	bool loaded() { return mStatus == WIN_VALID; };
	bool shouldClose();
	int getWidth() { return mWidth; };
	int getHeight() { return mHeight; };
	GLFWwindow* getGLFWWindow() { return mWindow; };
	void closeWindow();
	void changeCursorShape(CursorShape shape);
	//CALLBACKS

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset);
};