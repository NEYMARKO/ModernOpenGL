#include "PhysicsObject.h"
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() :
	mBroadPhase{ new btDbvtBroadphase() }, mCollisionConfig{ new btDefaultCollisionConfiguration() },
	mCollisionDispatcher{ new btCollisionDispatcher(mCollisionConfig) }, 
	mSolver{ new btSequentialImpulseConstraintSolver() }, 
	mDynamicsWorld { new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadPhase, mSolver, mCollisionConfig) }
{
	mDynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}
PhysicsWorld::~PhysicsWorld()
{
	for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		mDynamicsWorld->removeCollisionObject(obj);
	}

	// Only reference to unique pointers - destructor will be called
	mPhysicsObjects.clear();

	// Delete bullet world setup
	delete mDynamicsWorld;
	delete mSolver;
	delete mCollisionDispatcher;
	delete mCollisionConfig;
	delete mBroadPhase;
}

void PhysicsWorld::loadDefaultSimulation()
{
	auto p1{ std::make_unique<PhysicsObject>(glm::vec3(0.0f, -1.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShape::PLANE, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) };
	auto p2{ std::make_unique<PhysicsObject>(glm::vec3(0.0f, 50.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShape::SPHERE, 0.2f) };

	addObjectToWorld(std::move(p1));
	addObjectToWorld(std::move(p2));

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

void PhysicsWorld::addObjectToWorld(std::unique_ptr<PhysicsObject> object)
{
	mDynamicsWorld->addRigidBody(object.get()->getRigidBody());
	mPhysicsObjects.push_back(std::move(object));
}