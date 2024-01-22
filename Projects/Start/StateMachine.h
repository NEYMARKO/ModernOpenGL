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
		State state;
		SubState subState;
		Mesh* target;
		bool followMouse = false;

		glm::vec4 mouseStartWorld;
		glm::vec3 mouseDirectionWorld;

		Plane objectPlane;
		Camera* camera;
	public:
		std::vector<Mesh*>& objectsInScene;
		StateMachine(Mesh* mesh, Camera* camera, std::vector<Mesh*>& objectsInScene);
		void ChangeState(GLFWwindow* window, const int key, const int action, Camera& camera);
		void ControlState(GLFWwindow* window);
		
		void CalculateObjectPlane();
		glm::vec3 CalculateIntersectionPoint();
		void Click(GLFWwindow* window, Camera& camera, std::vector<MeshLoader*>& meshLoaders, int button, int action);
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