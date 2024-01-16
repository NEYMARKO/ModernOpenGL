#pragma once
#include "Camera.h"
#include "Mesh.h"

enum State
{
	NOTHING,
	GRAB,
	ROTATE,
	SCALE,
	ADD,
	DELETE,
	FOCUS,
	CLOSE_WINDOW
};

struct Plane
{
	float A;
	float B;
	float C;
	float D;
};

class StateMachine
{
	private:
		State state;
		Mesh* target;
		std::vector<Mesh*> objectsInScene;
		bool followMouse = false;

		glm::vec4 mouseStartWorld;
		glm::vec4 mouseEndWorld;

		Plane objectPlane;
		Camera* camera;
	public:
		StateMachine(Mesh* mesh, Camera* camera);
		void ChangeState(GLFWwindow* window, const int key, const int action, Camera& camera);
		void ControlState(GLFWwindow* window);
		
		void CalculateObjectPlane();
		void Click(GLFWwindow* window, Camera& camera, std::vector<Mesh*>& objectsInScene, MeshLoader* meshLoaderObj, int button, int action);
		void MouseMove(GLFWwindow* window, Camera& camera, const double mouseX, const double mouseY);
		//Checks if any object has been clicked on, sets object as target, returns target pointer for storage
		void CheckTarget();
		bool ShouldFollowMouse();
		void Grab();
		void Rotate();
		void Scale();
		void Add();
		void Delete();
		void CloseWindow(GLFWwindow* window);
		~StateMachine();
};