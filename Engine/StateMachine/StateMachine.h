#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>
//#include "OpenGLIncludes.h"
#include "../Window.h"
#include "./States/State.h"
#include "../SceneEntity.h"
#include "../Object.h"
//#include "Camera.h"
class Camera;
class Lighting;
class Ray;
//class Object;
//struct SceneEntity;
class PhysicsWorld;


class StateMachine
{
	private:

		std::unique_ptr<State> m_activeState;


	public:
		int m_lastKey;
		Window* m_window;
		Camera* m_camera;
		Lighting* m_lightSource;
		double mousePosX;
		double mousePosY;
		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;
		float m_rayLen{ 100.0f };
		//std::vector<std::unique_ptr<Object>>& m_objectsInScene;
		std::vector<std::unique_ptr<SceneEntity>>& m_objectsInScene;
		//Object* m_target;
		SceneEntity* m_target{ nullptr };
		btRigidBody* m_rbTarget{ nullptr };
		btVector3 m_rbPickPos;
		PhysicsWorld* m_physicsWorld;
		StateMachine(Window* window, Camera* camera, SceneEntity* lightSource,
			std::vector<std::unique_ptr<SceneEntity>>& objectsInScene,
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