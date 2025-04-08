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
	auto p1{ std::make_unique<PhysicsObject>(btVector3(0.0f, -1.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShapeType::PLANE, btVector3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.8f) };
	p1.get()->setRestitution(0.8f);
	addObjectToWorld(std::move(p1));
	auto p2{ std::make_unique<PhysicsObject>(btVector3(0.0f, 50.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShapeType::SPHERE, btVector3(0.0f, 0.0f, 0.0f), 0.2f, 1.5f, 0.8f) };
	p2.get()->setRestitution(0.8f);
	addObjectToWorld(std::move(p2));
	auto p3{ std::make_unique<PhysicsObject>(btVector3(0.0f, 70.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
		CollisionShapeType::SPHERE, btVector3(0.0f, 0.0f, 0.0f), 0.2f, 1.5f, 0.8f) };
	p3.get()->setRestitution(0.2f);
	addObjectToWorld(std::move(p3));

	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	//// Create ground rigid body
	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	//btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	//btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	//// Add to world
	//mDynamicsWorld->addRigidBody(groundRigidBody);

	//btCollisionShape* sphereShape = new btSphereShape(1.0f); // Radius of 1.0
	//btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0))); // Start at (0,10,0)

	//btScalar mass = 0.2f;
	//btVector3 inertia(0, 0, 0);
	//sphereShape->calculateLocalInertia(mass, inertia);

	//btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, inertia);
	//btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);

	//mDynamicsWorld->addRigidBody(sphereRigidBody);

	//sphereRigidBody->setRestitution(0.8f);   // Higher values (0.8 - 1.0) make it bounce more
	//groundRigidBody->setRestitution(0.8f);
	//p2.get()->setRestitution(0.8f);
	
	/*PhysicsObject* spherePhysicsObject = p2.get();
	btTransform transform;
	spherePhysicsObject->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	std::cout << "Sphere Pos RIGHT AFTER CONSTRUCTION: "
		<< pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << std::endl;

	btVector3 inertia = p2->getInertia();
	std::cout << "SPHERE INERTIA: " << inertia.getX() << " " << inertia.getY() << " " << inertia.getZ() << std::endl;*/
	/*mPhysicsObjects.push_back(std::move(p1));
	mPhysicsObjects.push_back(std::move(p2));*/
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

	//std::cout << "DELTA TIME: " << mDeltaTime << std::endl;
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
	mPhysicsObjects.push_back(std::move(object));
	if (!object) std::cout << "Object is nullptr" << std::endl;
	mDynamicsWorld->addRigidBody(mPhysicsObjects.back().get()->getRigidBody());
}

std::vector<std::unique_ptr<PhysicsObject>>* PhysicsWorld::getPhysicsObjects()
{
	return &mPhysicsObjects;
}