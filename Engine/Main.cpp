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
	auto lightBulb = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&lightBulbLoader, glm::vec3(-5.0f, 4.0f, 0.0f));
	Lighting light(&lightingShaderProgram, lightBulb.get(), glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	Scene scene{&camera, &light, objectsInScene, meshLoaders, &defaultShaderProgram, &boundingBoxShaderProgram};
	scene.addObject(std::move(lightBulb));

	MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader jointLoader("joint4.txt");

	auto cube = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&cubeLoader, glm::vec3(-30.0f, 0.0f, 0.0f));
	auto joint = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&jointLoader, glm::vec3(0.0f, 0.0f, 0.0f));

	KinematicChain ikChain(7, 45.0f, glm::vec3(0.0f, 0.0f, 0.0f), joint.get(), cube.get(), &gizmos);
	//AKO SE OVO STAVI PRIJE DEFINICIJE IK CHAIN-A, DOLAZI DO PROBLEMA JER SU JOINT I CUBE MOVED
	scene.addObject(std::move(joint));
	scene.addObject(std::move(cube));


	Mesh* jointTarget;
	glm::vec3 jointTargetPosition;

	PhysicsWorld physicsWorld{};

	while (!glfwWindowShouldClose(window.getGLFWWindow()))
	{
		scene.renderScene();

		grid.Draw(boundingBoxShaderProgram, camera);
		
		if (camera.mRay != nullptr)
		{
			camera.mRay->Draw(boundingBoxShaderProgram, camera);
		}

		ikChain.FabrikAlgorithm(10);

		//Move joint to it's new position and render it
		
		std::vector<std::unique_ptr<Joint>>* joints = ikChain.GetAllJoints();
		for (int i = 0; i < joints->size(); i++)
		{
			Joint* joint = (*joints)[i].get();
			jointTargetPosition = (joint->GetChild() == nullptr ? ikChain.GetTarget()->GetPosition() : joint->GetChild()->GetPosition());
			joint->GetMeshContainer()->Translate(joint->GetPosition());
			joint->RotateTowardsTarget(jointTargetPosition);
			std::string name = "j" + std::to_string(joint->GetID());
			gizmos.UpdateLine(name, joint->GetPosition(), joint->GetForwardVector(), 4);
			//gizmos.RenderBoundingBox(joint->GetMeshContainer()->boundingBox);
			joint->GetMeshContainer()->Render(camera, light);
			//joint->GetMeshContainer()->boundingBox->Draw(boundingBoxShaderProgram, camera);
		}
		camera.Move(window.getGLFWWindow(), physicsWorld.getDeltaTime());

		glfwSwapBuffers(window.getGLFWWindow());
		glfwPollEvents();

		physicsWorld.simulate();

		//btRigidBody* spherePhysicsObject = (*physicsWorld.getPhysicsObjects())[1].get()->getRigidBody();
		btRigidBody* spherePhysicsObject = btRigidBody::upcast(physicsWorld.getDynamicsWorld()->getCollisionObjectArray()[1]);
		btRigidBody* spherePhysicsObject2 = btRigidBody::upcast(physicsWorld.getDynamicsWorld()->getCollisionObjectArray()[2]);

		////std::cout << "IS SPHERE: " << spherePhysicsObject->checkIfSphere() << std::endl;
		//// Get updated object position
		btTransform transform;
		spherePhysicsObject->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		btTransform transform2;
		spherePhysicsObject2->getMotionState()->getWorldTransform(transform2);
		btVector3 pos2 = transform2.getOrigin();
		//std::cout << "Sphere Pos: " << pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << std::endl;
		glm::vec3 newPos = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
		glm::vec3 newPos2 = glm::vec3(pos2.getX(), pos2.getY(), pos2.getZ());
		objectsInScene[2]->Translate(newPos);
		objectsInScene[1]->Translate(newPos2);
		/*btVector3 vel = spherePhysicsObject->getRigidBody()->getLinearVelocity();
		std::cout << "Velocity after sim: " << vel.getX() << ", " << vel.getY() << ", " << vel.getZ() << std::endl;*/
	}

	defaultShaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	return 0;
}