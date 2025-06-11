#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE
//#include "OpenGLIncludes.h"
#include "State.h"
//#include "Camera.h"
class Camera;
class Ray;
class Shader;
class Object;
class MeshLoader;
class PhysicsWorld;


class StateMachine
{
	private:

		std::unique_ptr<State> m_activeState;

		Shader* mShaderProgram;

		PhysicsWorld* m_physicsWorld;

		std::vector<std::unique_ptr<MeshLoader>>& mMeshLoaders;
		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;

	public:
		int m_lastKey;
		Camera* m_camera;
		double mousePosX;
		double mousePosY;
		std::vector<std::unique_ptr<Object>>& m_objectsInScene;
		Object* m_target;
		StateMachine(Camera* m_camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Object>>& objectsInScene,
			PhysicsWorld* physicsWorld);
		
		void AddShaderPrograms(Shader* shader, Shader* boxShader);
		//Controls state that StateMachine is currently in. SM_State changes on the press of a button
		void KeyboardPress(GLFWwindow* window, int key, int action, Camera* camera);
		//Callback function for mouse click
		void MouseClick(GLFWwindow* window, Camera* camera, int button, int action);
		//Callback function for mouse movement
		void MouseMove(GLFWwindow* window, Camera* camera, double mouseX, double mouseY);
		
		void update();
		void changeState();

		//Adds object to scene at the position of the click
		//Different objects can be added by pressing numbers 1-8 while in ADD mode
		void AddObject(Ray* ray);
		//Deletes selected object from both the scene and the vector containing all objects in the scene
		void DeleteObject();
		void CloseWindow(GLFWwindow* window);
		//~StateMachine();
};