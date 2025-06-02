#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "State.h"
#include "Camera.h"
//class State;
class Shader;
class Object;
class MeshLoader;
class PhysicsWorld;

enum SM_State
{
	SM_NOTHING,
	SM_GRAB,
	SM_ROTATE,
	SM_SCALE,
	SM_ADD,
	SM_DELETE,
	SM_FOCUS,
	SM_RESTART_SCENE,
	SM_CLOSE_WINDOW
};

enum SubState
{
	NO_1,
	NO_2,
	NO_3,
	NO_4,
	NO_5,
	NO_6,
	NO_7,
	NO_8,
	X,
	Y,
	Z,
	SM_EMPTY
};

struct Plane
{
	glm::vec3 normal;
	float D;
};

class StateMachine
{
	private:

		std::unique_ptr<State> m_activeState;

		Shader* mShaderProgram;

		SM_State state;
		SubState subState;
		Plane objectPlane;
		PhysicsWorld* m_physicsWorld;

		std::vector<std::unique_ptr<MeshLoader>>& mMeshLoaders;
		bool followMouse = false;
		bool canRotateCamera = false;
		double mousePosX;
		double mousePosY;
		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;

	public:
		Camera* m_camera;
		std::vector<std::unique_ptr<Object>>& m_objectsInScene;
		Object* m_target;
		StateMachine(Camera* m_camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Object>>& objectsInScene,
			PhysicsWorld* physicsWorld);
		
		void AddShaderPrograms(Shader* shader, Shader* boxShader);
		//Controls state that StateMachine is currently in. SM_State changes on the press of a button
		void ChangeState(GLFWwindow* window, int key, int action, Camera* camera);
		//Calculates coefficients of a plane in which target object lies, normal of a plane is opposite to the camera direction
		void CalculateObjectPlane();
		//Calculates intersection between plane and a point
		//Used for finding intersection of plane (in which target object lies) and click ray projected from camera
		glm::vec3 CalculateIntersectionPoint();
		//Callback function for mouse click
		void MouseClick(GLFWwindow* window, Camera* camera, int button, int action);
		//Callback function for mouse movement
		void MouseMove(GLFWwindow* window, Camera* camera, double mouseX, double mouseY);
		
		//Returns vector containing pointers to all objects in the scene
		//Checks if any object has been clicked on
		void CheckTarget();
		bool ShouldFollowMouse();
		void Grab();
		void Rotate();
		void Scale();

		//Adds object to scene at the position of the click
		//Different objects can be added by pressing numbers 1-8 while in ADD mode
		void AddObject(Ray* ray);
		//Deletes selected object from both the scene and the vector containing all objects in the scene
		void DeleteObject();
		void CloseWindow(GLFWwindow* window);
		//~StateMachine();
};