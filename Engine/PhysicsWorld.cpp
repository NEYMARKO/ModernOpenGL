#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() :
	mBroadPhase{ new btDbvtBroadphase() }, mCollisionConfig{ new btDefaultCollisionConfiguration() },
	mCollisionDispatcher{ new btCollisionDispatcher(mCollisionConfig) }, 
	mSolver{ new btSequentialImpulseConstraintSolver() }, 
	mDynamicsWorld { new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadPhase, mSolver, mCollisionConfig) }
{
}
PhysicsWorld::~PhysicsWorld()
{
	for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		mDynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	delete mDynamicsWorld;
	delete mSolver;
	delete mCollisionDispatcher;
	delete mCollisionConfig;
	delete mBroadPhase;
}

void PhysicsWorld::updateDeltaTime()
{
	mDeltaTime = mCurrentFrame - mLastFrame;
	if (mDeltaTime > 10 * mFixedTimeStep)
		mDeltaTime = 10 * mFixedTimeStep;
}

void PhysicsWorld::simulate()
{
	mCurrentFrame = static_cast<float>(glfwGetTime());
	updateDeltaTime();
	mLastFrame = mCurrentFrame;

	mAccumulator += mDeltaTime;
	while (mAccumulator >= mFixedTimeStep)
	{
		mDynamicsWorld->stepSimulation(mFixedTimeStep, 10); // Perform physics updates at 60Hz
		mAccumulator -= mFixedTimeStep;
	}
}