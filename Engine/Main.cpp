#include "Scene.h"
#include "./StateMachine/StateMachine.h"
#include "Camera.h"
#include "Lighting.h"
#include "Grid.h"
#include "Gizmos.h"
#include "Window.h"
#include <memory>
#include "PhysicsWorld.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Ray.h"
#include "Ragdoll.h"
#include "BulletGizmos.h"
#include "SceneEntity.h"

#include <filesystem>

int main()
{
	std::vector<std::unique_ptr<SceneEntity>> objectsInScene;
	std::vector<std::unique_ptr<MeshLoader>> meshLoaders;

	Camera camera(glm::vec3(-5.0f, 5.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 1, 800, 800);

	Window window{ &camera, 1920, 1080 };
	if (!window.loaded()) return -1;

	Shader defaultShaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");
	Shader boundingBoxShaderProgram("borderBox.vert", "borderBox.frag");
	Shader pointShader("point.vert", "point.frag");

	PhysicsWorld physicsWorld{};

	BulletGizmos bulletGizmos(&physicsWorld);


	Grid grid(100);

	MeshLoader lightBulbLoader("lightBulb.txt");

	ResourceManager<Mesh> m_meshResourceManager;
	ResourceManager<Material> m_materialResourceManager;
	ResourceManager<Shader> m_shaderResourceManager;

	ResourceManager<Shader>::addResource("gizmos", std::make_unique<Shader>("gizmos.vert", "gizmos.frag"));
	ResourceManager<Shader>::addResource("texture", std::make_unique<Shader>(
		"texture.vert", "texture.frag"
	));
	auto lightBulbTransform = Transform(glm::vec3(-5.0f, 7.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

	/*auto lightBulb = ResourceManager<Mesh>::addResource("lightBulb", std::make_unique<Mesh>(&lightBulbLoader));

	auto lightBulbMaterial = ResourceManager<Material>::addResource("lightBulb", std::make_unique<Material>(&defaultShaderProgram));

	auto lightBulbRenderer = MeshRenderer(lightBulb, lightBulbMaterial);*/
	/*Lighting light(&lightingShaderProgram, glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));*/

	objectsInScene.push_back(std::make_unique<Lighting>(&lightingShaderProgram, glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	//auto* o = objectsInScene.back();
	StateMachine stateMachine(&window, &camera,
		objectsInScene.back().get(), objectsInScene, &physicsWorld);
	//stateMachine.AddShaderPrograms(&defaultShaderProgram, &boundingBoxShaderProgram);
	window.addStateMachine(&stateMachine);
	Scene scene{ &camera,
		dynamic_cast<Lighting*>(objectsInScene.back().get()), objectsInScene, &defaultShaderProgram };

	const int ragdoll_count = 5;
	std::vector<std::unique_ptr<Ragdoll>> rags;
	for (int i = 0; i < ragdoll_count; i++)
	{
		rags.push_back(std::make_unique<Ragdoll>(
			glm::vec3(0.0f, 10.0f + i * 2.0f, -3.0f),
			&physicsWorld,
			2.5f
		));

		scene.addRagdoll(rags.back().get());
	}
	/*Ragdoll ragdoll(glm::vec3(0.0f, 35.0f, -3.0f), &physicsWorld, 2.5);
	Ragdoll ragdoll2(glm::vec3(0.0f, 12.0f, -2.5f), &physicsWorld, 2.5);
	scene.addRagdoll(&ragdoll);
	scene.addRagdoll(&ragdoll2);*/
	
	Gizmos gizmos(&camera);

	for (auto& obj : objectsInScene)
	{
		//std::cout << "OBJECT: " << obj.get()->getName() << '\n';
		gizmos.addEditorCollider(&obj.get()->m_editorCollider);
	}
	/*auto lightBulbObject = std::make_unique<Object>(std::move(lightBulbTransform), std::move(lightBulbRenderer));*/
	gizmos.addEditorCollider(&objectsInScene.back().get()->m_editorCollider);
	//scene.addObject(std::move(lightBulbObject));

	std::filesystem::path p = std::filesystem::current_path() / ".." / ".." / ".." / "quit.flag";
	float fps = 0;
	//std::cout << "Path: " << p.string() << "\n";
	glfwSwapInterval(0); // disable vsync
	while (!window.shouldClose() && !std::filesystem::exists(p))
	{
		scene.renderScene();
		/*for (auto& obj : objectsInScene)
			obj.get()->getEditorCollider()->setupAABB();*/
		gizmos.updateBufferContent();
		gizmos.renderBoundingVolumes();
		bulletGizmos.updateBufferContent();
		bulletGizmos.renderColliders(&camera);
		grid.Draw(boundingBoxShaderProgram, camera);
		scene.renderRagdoll();
		//ragdoll.update();

		//light.m_editorCollider.setupAABB();
		fps = 1 / physicsWorld.getDeltaTime();
		if (fps > 0.0f && fps < 10000.0f) std::cout << fps << "\n";
		if (camera.mRay != nullptr)
		{
			camera.mRay->Draw(boundingBoxShaderProgram, camera);
		}

		stateMachine.update();

		glfwSwapBuffers(window.getGLFWWindow());
		glfwPollEvents();

		physicsWorld.fixedUpdate();
	}
	//defaultShaderProgram.Delete();
	//lightingShaderProgram.Delete();
	//boundingBoxShaderProgram.Delete();
	//pointShader.Delete();
	return 0;
}