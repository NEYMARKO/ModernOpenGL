#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
//#include "OpenGLIncludes.h"
#include "Window.h"
#include "State.h"
//#include "Camera.h"
class Camera;
class Ray;
class Object;
class PhysicsWorld;


class StateMachine
{
	private:

		std::unique_ptr<State> m_activeState;

		PhysicsWorld* m_physicsWorld;

		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;


	public:
		int m_lastKey;
		Window* m_window;
		Camera* m_camera;
		double mousePosX;
		double mousePosY;
		std::vector<std::unique_ptr<Object>>& m_objectsInScene;
		Object* m_target;
		StateMachine(Window* window, Camera* m_camera, std::vector<std::unique_ptr<Object>>& objectsInScene,
			PhysicsWorld* physicsWorld);
		
		//Controls state that StateMachine is currently in. SM_State changes on the press of a button
		void KeyboardPress(int key, int action, Camera* camera);
		//Callback function for mouse click
		void MouseClick(Camera* camera, int button, int action);
		//Callback function for mouse movement
		void MouseMove(Camera* camera, double mouseX, double mouseY);
		
		void update();
		void changeState();

		//Adds object to scene at the position of the click
		//Different objects can be added by pressing numbers 1-8 while in ADD mode
		void AddObject(Ray* ray);
		//Deletes selected object from both the scene and the vector containing all objects in the scene
		void DeleteObject();
		void CloseWindow();
		//~StateMachine();
};