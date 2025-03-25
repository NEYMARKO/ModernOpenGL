#include "Camera.h"
#include "Lighting.h"
#include "StateMachine.h"
#include "Grid.h"
#include "KinematicChain.h"
#include "Gizmos.h"
#include "Window.h"
#include <btBulletDynamicsCommon.h>


//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset);

//int globalWidth = 800, globalHeight= 800;
float deltaTime = 0.0f, lastFrame = 0.0f;

Camera globalCamera(glm::vec3(-15.0f, 0.0f, -40.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.5f, 1.0f, 800, 800);

std::vector<Mesh*> objectsInScene;
std::vector<MeshLoader*> meshLoaders;

StateMachine stateMachine(nullptr, &globalCamera, meshLoaders, objectsInScene);


int main()
{
	Window window{&globalCamera, 800, 800};

	if (!window.loaded()) return -1;
	window.addStateMachine(&stateMachine);
	
	/*glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Intro to Modern OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}*/
	//glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursor_position_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetScrollCallback(window, mouse_scroll_back);
	////Load GLAD so it configures OpenGL
	//gladLoadGL();
	//glViewport(0, 0, globalWidth, globalHeight);

	Shader shaderProgram("default.vert", "default.frag");
	Shader lightingShaderProgram("lighting.vert", "lighting.frag");
	Shader boundingBoxShaderProgram("borderBox.vert", "borderBox.frag");
	Shader pointShader("point.vert", "point.frag");
	
	stateMachine.AddShaderPrograms(&shaderProgram, &boundingBoxShaderProgram);

	MeshLoader lightBulbLoader("lightBulb.txt");
	MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader dragonLoader("dragonSmooth.txt");
	MeshLoader templeLoader("templeFlat.txt");
	MeshLoader frogLoader("frogSmooth.txt");
	MeshLoader teddyLoader("teddyFlat.txt");
	MeshLoader sphereLoader("sphere.txt");
	//BLENDER: rotate around X for 270 (-90) degrees,	EXPORT: forward: -X, up: Z
	MeshLoader coneLoader("cone.txt");
	//forward: -X, up: Z - file: joint.blend
	MeshLoader jointLoader("joint4.txt");

	meshLoaders.push_back(&cubeLoader);
	meshLoaders.push_back(&dragonLoader);
	meshLoaders.push_back(&templeLoader);
	meshLoaders.push_back(&frogLoader);
	meshLoaders.push_back(&teddyLoader);
	meshLoaders.push_back(&sphereLoader);
	meshLoaders.push_back(&coneLoader);
	meshLoaders.push_back(&jointLoader);

	float id = 0;

	Mesh* lightBulb = new Mesh(&shaderProgram, &boundingBoxShaderProgram, &lightBulbLoader, glm::vec3(-5.0f, 4.0f, 0.0f), id++);
	Mesh* dragon = new Mesh(&shaderProgram, &boundingBoxShaderProgram, &dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	Mesh* cube = new Mesh(&shaderProgram, &boundingBoxShaderProgram, &cubeLoader, glm::vec3(-30.0f, 0.0f, 0.0f), id++);
	Mesh* sphere = new Mesh(&shaderProgram, &boundingBoxShaderProgram, &sphereLoader, glm::vec3(-35.0f, 0.0f, 0.0f), id++);
	//Mesh* cone = new Mesh(&coneLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	Mesh* joint = new Mesh(&shaderProgram, &boundingBoxShaderProgram, &jointLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);

	objectsInScene.push_back(lightBulb);
	//objectsInScene.push_back(dragon);
	objectsInScene.push_back(cube);
	objectsInScene.push_back(sphere);
	//objectsInScene.push_back(cone);
	//objectsInScene.push_back(joint);

	Lighting light(*lightBulb, glm::vec3(1.0f, 1.0f, 1.0f));

	Grid grid(100);

	Gizmos gizmos(&globalCamera, &shaderProgram, &boundingBoxShaderProgram, &pointShader);
	
	KinematicChain ikChain(7, 45.0f, glm::vec3(0.0f, 0.0f, 0.0f), joint, cube, &gizmos);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





	const float fixedTimeStep = 1.0f / 60.0f; // Fixed 60Hz physics step
	float accumulator = 0.0f;
	//float _lastFrame = static_cast<float>(glfwGetTime());
	float _lastFrame = 0.0f;
	objectsInScene[2]->Translate(glm::vec3(0, 50, 0));
	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//std::cout << "TIME SINCE LAST FRAME: " << deltaTime << std::endl;
		glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		light.Draw(lightingShaderProgram, boundingBoxShaderProgram, globalCamera);

		grid.Draw(boundingBoxShaderProgram, globalCamera);
		
		for (int i = 0; i < (*stateMachine.GetObjectsInScene()).size(); i++)
		{
			(*stateMachine.GetObjectsInScene())[i]->Render(globalCamera, light);
			(*stateMachine.GetObjectsInScene())[i]->boundingBox->Draw(boundingBoxShaderProgram, globalCamera);
		}
		
		if (globalCamera.ray != nullptr)
		{
			globalCamera.ray->Draw(boundingBoxShaderProgram, globalCamera);
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
			joint->GetMeshContainer()->Render(globalCamera, light);
			points.push_back(joint->GetPosition());
			//joint->GetMeshContainer()->boundingBox->Draw(boundingBoxShaderProgram, globalCamera);
		}

		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		//gizmos.RenderAllLines(color);
		//gizmos.RenderPoints(10.0f);
		gizmos.UpdatePoints(points);
		points.clear();
		globalCamera.Move(window.getGLFWwindow(), deltaTime);

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
		(*stateMachine.GetObjectsInScene())[2]->Translate(newPos);
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

	shaderProgram.Delete();
	lightingShaderProgram.Delete();
	boundingBoxShaderProgram.Delete();
	/*glfwDestroyWindow(window);
	glfwTerminate();*/
	return 0;
}

//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	stateMachine.ChangeState(window, key, action, globalCamera);
//}
//
//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	stateMachine.MouseMove(window, globalCamera, xpos, ypos);
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	stateMachine.MouseClick(window, globalCamera, button, action);
//}
//
//void mouse_scroll_back(GLFWwindow* window, double xoffset, double yoffset)
//{
//	globalCamera.Zoom(yoffset);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glfwGetWindowSize(window, &globalWidth, &globalHeight);
//	globalCamera.UpdateViewportDimensions(globalWidth, globalHeight);
//	glViewport(0, 0, globalWidth, globalHeight);
//} 