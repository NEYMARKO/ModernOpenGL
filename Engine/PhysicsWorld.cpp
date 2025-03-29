#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() :
	broadphase{ new btDbvtBroadphase() }, collisionConfig{ new btDefaultCollisionConfiguration() },
	collisionDispatcher{ new btCollisionDispatcher(collisionConfig) }, 
	solver{ new btSequentialImpulseConstraintSolver() }, 
	dynamicsWorld { new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, collisionConfig) }
{
}
PhysicsWorld::~PhysicsWorld()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	delete dynamicsWorld;
	delete solver;
	delete collisionDispatcher;
	delete collisionConfig;
	delete broadphase;
}

void PhysicsWorld::updateDeltaTime()
{
	deltaTime = currentFrame - lastFrame;
	if (deltaTime > 10 * fixedTimeStep)
		deltaTime = 10 * fixedTimeStep;
}

void PhysicsWorld::simulate()
{
	currentFrame = static_cast<float>(glfwGetTime());
	updateDeltaTime();
	lastFrame = currentFrame;

	accumulator += deltaTime;
	while (accumulator >= fixedTimeStep)
	{
		dynamicsWorld->stepSimulation(fixedTimeStep, 10); // Perform physics updates at 60Hz
		accumulator -= fixedTimeStep;
	}
}