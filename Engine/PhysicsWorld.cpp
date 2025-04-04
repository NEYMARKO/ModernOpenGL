#include "PhysicsObject.h"
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() :
	mBroadPhase{ new btDbvtBroadphase() }, mCollisionConfig{ new btDefaultCollisionConfiguration() },
	mCollisionDispatcher{ new btCollisionDispatcher(mCollisionConfig) }, 
	mSolver{ new btSequentialImpulseConstraintSolver() }, 
	mDynamicsWorld { new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadPhase, mSolver, mCollisionConfig) }
{
	mDynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	loadDefaultSimulation();
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
		CollisionShape::PLANE, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f) };
	p1.get()->setRestitution(0.8f);
	auto p2{ std::make_unique<PhysicsObject>(glm::vec3(0.0f, 50.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShape::SPHERE, glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 1.0f) };
	p2.get()->setRestitution(0.8f);
	
	PhysicsObject* spherePhysicsObject = p2.get();
	btTransform transform;
	spherePhysicsObject->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	std::cout << "Sphere Pos RIGHT AFTER CONSTRUCTION: "
		<< pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << std::endl;

	btVector3 inertia = p2->getInertia();
	std::cout << "SPHERE INERTIA: " << inertia.getX() << " " << inertia.getY() << " " << inertia.getZ() << std::endl;
	/*mPhysicsObjects.push_back(std::move(p1));
	mPhysicsObjects.push_back(std::move(p2));*/
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
	/*PhysicsObject* spherePhysicsObject = mPhysicsObjects[1].get();
	btTransform transform;
	spherePhysicsObject->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	std::cout << "SPHERE IN SIMULATE: "
		<< pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << std::endl;*/
	/*if (mDynamicsWorld->getNumCollisionObjects() < 2)
	{
		mDynamicsWorld->addRigidBody(mPhysicsObjects[0].get()->getRigidBody());
		mDynamicsWorld->addRigidBody(mPhysicsObjects[1].get()->getRigidBody());
	}*/
	mCurrentFrame = static_cast<float>(glfwGetTime());
	updateDeltaTime();
	mLastFrame = mCurrentFrame;

	std::cout << "DELTA TIME: " << mDeltaTime << std::endl;
	mAccumulator += mDeltaTime;
	while (mAccumulator >= mFixedTimeStep)
	{
		int steps = mDynamicsWorld->stepSimulation(mFixedTimeStep, 10); // Perform physics updates at 60Hz
		mAccumulator -= mFixedTimeStep;
		//std::cout << "Actual bullet steps: " << steps << std::endl;
	}
}

void PhysicsWorld::addObjectToWorld(std::unique_ptr<PhysicsObject> object)
{
	mDynamicsWorld->addRigidBody(object.get()->getRigidBody());
	mPhysicsObjects.push_back(std::move(object));
}

std::vector<std::unique_ptr<PhysicsObject>>* PhysicsWorld::getPhysicsObjects()
{
	return &mPhysicsObjects;
}