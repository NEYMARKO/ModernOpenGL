#include "Camera.h"
#include "Lighting.h"
#include "StateMachine.h"
#include "Grid.h"
#include "KinematicChain.h"
#include "Gizmos.h"
#include "Window.h"
#include "Scene.h"
#include <memory>
#include <btBulletDynamicsCommon.h>



int main()
{
	std::vector<std::unique_ptr<Mesh>> objectsInScene;
	std::vector<std::unique_ptr<MeshLoader>> meshLoaders;

	Camera camera(glm::vec3(-15.0f, 0.0f, -40.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 1.0f, 800, 800);
	
	Window window{&camera, 800, 800};
	if (!window.loaded()) return -1;

	Shader defaultShaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");
	Shader boundingBoxShaderProgram("borderBox.vert", "borderBox.frag");
	Shader pointShader("point.vert", "point.frag");
	
	Gizmos gizmos(&camera, &defaultShaderProgram, &boundingBoxShaderProgram, &pointShader);
	StateMachine stateMachine(nullptr, &camera, meshLoaders, objectsInScene);
	stateMachine.AddShaderPrograms(&defaultShaderProgram, &boundingBoxShaderProgram);
	
	window.addStateMachine(&stateMachine);
	float deltaTime = 0.0f, lastFrame = 0.0f;

	MeshLoader lightBulbLoader("lightBulb.txt");
	auto lightBulb = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&lightBulbLoader, glm::vec3(-5.0f, 4.0f, 0.0f));
	Lighting light(lightBulb.get(), glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	Scene scene{&camera, &light, objectsInScene, meshLoaders, &defaultShaderProgram, &boundingBoxShaderProgram};
	scene.addObject(std::move(lightBulb));

	MeshLoader cubeLoader("cubeFlat.txt");

	MeshLoader jointLoader("joint4.txt");

	auto cube = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&cubeLoader, glm::vec3(-30.0f, 0.0f, 0.0f));
	auto joint = std::make_unique<Mesh>(&defaultShaderProgram, &boundingBoxShaderProgram, 
		&jointLoader, glm::vec3(0.0f, 0.0f, 0.0f));
	KinematicChain ikChain(7, 45.0f, glm::vec3(0.0f, 0.0f, 0.0f), joint.get(), cube.get(), &gizmos);

	scene.addObject(std::move(joint));
	scene.addObject(std::move(cube));

	Grid grid(100);

	Mesh* jointTarget;
	glm::vec3 jointTargetPosition;

	std::vector<glm::vec3> points;


	//BULLET PHYSICS
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	// Create ground shape
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	// Create ground rigid body
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	// Add to world
	dynamicsWorld->addRigidBody(groundRigidBody);

	// Create box shape
	btCollisionShape* sphereShape = new btSphereShape(1.0f); // Radius of 1.0
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0))); // Start at (0,10,0)

	btScalar mass = 0.2f;
	btVector3 inertia(0, 0, 0);
	sphereShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, inertia);
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);

	dynamicsWorld->addRigidBody(sphereRigidBody);

	//BOUNCINES
	sphereRigidBody->setRestitution(0.8f);   // Higher values (0.8 - 1.0) make it bounce more
	groundRigidBody->setRestitution(0.8f); // The ground also needs restitution
	//sphereRigidBody->setActivationState(DISABLE_DEACTIVATION); // Prevent it from sleeping

	const float fixedTimeStep = 1.0f / 60.0f; // Fixed 60Hz physics step
	float accumulator = 0.0f;
	//float _lastFrame = static_cast<float>(glfwGetTime());
	float _lastFrame = 0.0f;
	objectsInScene[2]->Translate(glm::vec3(0, 50, 0));
	
	std::cout << "MESHLOADERS IN SCENE: " << meshLoaders.size() << std::endl;
	//std::cout << "STATE MACHINE OBJECTS IN SCENE: " << (stateMachine.GetObjectsInScene())->size() << std::endl;
	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//std::cout << "TIME SINCE LAST FRAME: " << deltaTime << std::endl;
		/*glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		scene.renderObjects();

		light.Draw(lightingShaderProgram, boundingBoxShaderProgram, camera);

		grid.Draw(boundingBoxShaderProgram, camera);
		
		/*for (int i = 0; i < (*stateMachine.GetObjectsInScene()).size(); i++)
		{
			(*stateMachine.GetObjectsInScene())[i]->Render(camera, light);
			(*stateMachine.GetObjectsInScene())[i]->boundingBox->Draw(boundingBoxShaderProgram, camera);
		}*/
		
		if (camera.ray != nullptr)
		{
			camera.ray->Draw(boundingBoxShaderProgram, camera);
		}

		ikChain.FabrikAlgorithm(10);

		//Move joint to it's new position and render it
		
		for (Joint* joint : (*ikChain.GetAllJoints()))
		{
			jointTargetPosition = (joint->GetChild() == nullptr ? ikChain.GetTarget()->GetPosition() : joint->GetChild()->GetPosition());
			//jointTarget = (joint->GetChild() == nullptr ? ikChain.GetTarget() : joint->GetChild()->GetMeshContainer());
			//joint->RotateTowardsTarget(jointTarget->GetPosition());
			
			//only end is printing position, rest of them are printing NaN if they get rotated
			//joint->RotateTowardsTarget(ikChain.GetTarget()->objectPos);
			
			//PrintClass::PrintVec3(jointTargetPosition);
			
			/*std::cout << "JOINT POSITION: ";
			PrintClass::PrintVec3(joint->GetPosition());

			std::cout << "JOINT END: ";
			PrintClass::PrintVec3(joint->GetJointEnd());*/

			//if (joint->GetChild()) PrintClass::PrintVec3(joint->GetChild()->GetPosition());
			joint->GetMeshContainer()->Translate(joint->GetPosition());
			joint->RotateTowardsTarget(jointTargetPosition);
			std::string name = "j" + std::to_string(joint->GetID());
			gizmos.UpdateLine(name, joint->GetPosition(), joint->GetForwardVector(), 4);
			//gizmos.RenderBoundingBox(joint->GetMeshContainer()->boundingBox);
			joint->GetMeshContainer()->Render(camera, light);
			points.push_back(joint->GetPosition());
			//joint->GetMeshContainer()->boundingBox->Draw(boundingBoxShaderProgram, camera);
		}

		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		//gizmos.RenderAllLines(color);
		//gizmos.RenderPoints(10.0f);
		gizmos.UpdatePoints(points);
		points.clear();
		camera.Move(window.getGLFWwindow(), deltaTime);

		glfwSwapBuffers(window.getGLFWwindow());
		glfwPollEvents();


		float _currentFrame = static_cast<float>(glfwGetTime());
		float _deltaTime = _currentFrame - _lastFrame;
		_deltaTime = (_deltaTime > 3 * fixedTimeStep) ? 3 * fixedTimeStep : _deltaTime;
		//std::cout << "TIME SINCE LAST FRAME: " << _deltaTime << std::endl;
		_lastFrame = _currentFrame;

		accumulator += _deltaTime;

		//dynamicsWorld->stepSimulation(_deltaTime, 10);
		while (accumulator >= fixedTimeStep) {
			dynamicsWorld->stepSimulation(fixedTimeStep, 10); // Perform physics updates at 60Hz
			accumulator -= fixedTimeStep;
		}

		// Get updated object position
		btTransform transform;
		sphereRigidBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		glm::vec3 newPos = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
		objectsInScene[2]->Translate(newPos);
	}

	//BULLET CLEANUP
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	/*delete sphereRigidBody;
	delete sphereShape;
	delete groundRigidBody;
	delete groundShape;*/
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;

	defaultShaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	/*glfwDestroyWindow(window);
	glfwTerminate();*/
	return 0;
}