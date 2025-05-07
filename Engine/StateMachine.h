#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Shader;
class Object;
class MeshLoader;

enum State
{
	NOTHING,
	GRAB,
	ROTATE,
	SCALE,
	ADD,
	DELETE,
	FOCUS,
	RESTART_SCENE,
	CLOSE_WINDOW
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
	EMPTY
};

struct Plane
{
	glm::vec3 normal;
	float D;
};

class StateMachine
{
	private:

		Shader* mShaderProgram;
		Shader* mBoundingBoxShaderProgram;

		Camera* camera;
		//Mesh* mTarget;
		Object* mTarget;
		State state;
		SubState subState;
		Plane objectPlane;

		std::vector<std::unique_ptr<Object>>& mObjectsInScene;
		std::vector<std::unique_ptr<MeshLoader>>& mMeshLoaders;
		bool followMouse = false;
		bool canRotateCamera = false;
		double mousePosX;
		double mousePosY;
		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;

	public:
		StateMachine(Object* target, Camera* camera, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, std::vector<std::unique_ptr<Object>>& objectsInScene);
		
		void AddShaderPrograms(Shader* shader, Shader* boxShader);
		//Controls state that StateMachine is currently in. State changes on the press of a button
		void ChangeState(GLFWwindow* window, const int key, const int action, Camera& camera);
		//Calculates coefficients of a plane in which target object lies, normal of a plane is opposite to the camera direction
		void CalculateObjectPlane();
		//Calculates intersection between plane and a point
		//Used for finding intersection of plane (in which target object lies) and click ray projected from camera
		glm::vec3 CalculateIntersectionPoint();
		//Callback function for mouse click
		void MouseClick(GLFWwindow* window, Camera& camera, int button, int action);
		//Callback function for mouse movement
		void MouseMove(GLFWwindow* window, Camera& camera, const double mouseX, const double mouseY);
		
		//Returns vector containing pointers to all objects in the scene
		/*std::vector<std::unique_ptr<Mesh>> GetObjectsInScene();*/
		//Checks if any object has been clicked on
		void CheckTarget();
		bool ShouldFollowMouse();
		void Grab();
		void Rotate();
		void Scale();

		void SortObjectsInScene();
		void QuickSort(const int& low, const int& high);
		int Partition(const int& low, const int& high);
		void Swap(const int& firstPos, const int& secondPos);

		//Adds object to scene at the position of the click
		//Different objects can be added by pressing numbers 1-8 while in ADD mode
		void AddObject(Ray* ray);
		//Deletes selected object from both the scene and the vector containing all objects in the scene
		void DeleteObject();
		void CloseWindow(GLFWwindow* window);
		//~StateMachine();
};