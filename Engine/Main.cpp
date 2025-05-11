#include "Camera.h"
#include "Lighting.h"
#include "StateMachine.h"
#include "Grid.h"
#include "Joint.h"
#include "KinematicChain.h"
#include "Gizmos.h"
#include "Window.h"
#include "Scene.h"
#include <memory>
#include "PhysicsObject.h"
#include "PhysicsWorld.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Ray.h"

int main()
{
	std::vector<std::unique_ptr<Object>> objectsInScene;
	std::vector<std::unique_ptr<MeshLoader>> meshLoaders;

	Camera camera(glm::vec3(-5.0f, 5.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 0.5f, 800, 800);
	
	Window window{&camera, 1024, 1024};
	if (!window.loaded()) return -1;

	Shader defaultShaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");
	Shader boundingBoxShaderProgram("borderBox.vert", "borderBox.frag");
	Shader pointShader("point.vert", "point.frag");
	
	Gizmos gizmos(&camera, &defaultShaderProgram, &boundingBoxShaderProgram, &pointShader);
	StateMachine stateMachine(nullptr, &camera, meshLoaders, objectsInScene);
	stateMachine.AddShaderPrograms(&defaultShaderProgram, &boundingBoxShaderProgram);
	window.addStateMachine(&stateMachine);
	
	Grid grid(100);

	MeshLoader lightBulbLoader("lightBulb.txt");

	auto lightBulbTransform = std::make_unique<Transform>(glm::vec3(-5.0f, 4.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	
	auto lightBulb = std::make_unique<Mesh>(&boundingBoxShaderProgram, &lightBulbLoader);
	
	auto lightBulbRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(lightBulb), std::make_unique<Material>(&defaultShaderProgram));

	Lighting light(&lightingShaderProgram, lightBulb.get(), glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	Scene scene{&camera, &light, objectsInScene, /*meshLoaders,*/ &defaultShaderProgram, &boundingBoxShaderProgram};
	
	auto lightBulbObject = std::make_unique<Object>(std::move(lightBulbTransform), std::move(lightBulbRenderer));
	scene.addObject(std::move(lightBulbObject));

	PhysicsWorld physicsWorld{};

	while (!window.shouldClose())
	{
		scene.renderScene();

		grid.Draw(boundingBoxShaderProgram, camera);
		
		if (camera.mRay != nullptr)
		{
			camera.mRay->Draw(boundingBoxShaderProgram, camera);
		}

		camera.Move(window.getGLFWWindow(), physicsWorld.getDeltaTime());

		glfwSwapBuffers(window.getGLFWWindow());
		glfwPollEvents();

		physicsWorld.fixedUpdate();
	}

	defaultShaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	pointShader.Delete();
	return 0;
}