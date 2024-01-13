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

class StateMachine
{
	private:
		State state;
		Mesh* target;
		std::vector<Mesh*> objectsInScene;
	public:
		StateMachine(Mesh* mesh);
		void ChangeState(GLFWwindow* window, const int key, const int action);
		void ControlState(GLFWwindow* window);
		
		void Click(GLFWwindow* window, Camera& camera, std::vector<Mesh*>& objectsInScene, MeshLoader* meshLoaderObj, int button, int action);
		//Checks if any object has been clicked on, sets object as target, returns target pointer for storage
		void CheckTarget();

		void Grab();
		void Rotate();
		void Scale();
		void Add();
		void Delete();
		void CloseWindow(GLFWwindow* window);
		~StateMachine();
};