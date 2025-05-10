#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Object.h"
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
	m_rigidBodies.clear();

	// Delete bullet world setup
	delete mDynamicsWorld;
	delete mSolver;
	delete mCollisionDispatcher;
	delete mCollisionConfig;
	delete mBroadPhase;
}

void PhysicsWorld::loadDefaultSimulation()
{
	//auto p1{ std::make_unique<PhysicsObject>(0.0f, 0.8f, 0.0f, 
	//	btVector3(0.0f, 0.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
	//	btVector3(0.0f, 1.0f, 0.0f), CollisionShapeType::PLANE) };
	////p1.get()->setRestitution(0.8f);
	//addObjectToWorld(std::move(p1));
	//auto p2{ std::make_unique<PhysicsObject>(0.2f, 0.8f, 1.0f,
	//	btVector3(0.0f, 50.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
	//	 btVector3(0.0f, 0.0f, 0.0f), CollisionShapeType::SPHERE) };
	////p2.get()->setRestitution(0.8f);
	//addObjectToWorld(std::move(p2));
	//auto p3{ std::make_unique<PhysicsObject>(0.5f, 0.8f, 1.0f,
	//	btVector3(0.0f, 70.0f, 0.0f), btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
	//	 btVector3(0.0f, 0.0f, 0.0f), CollisionShapeType::SPHERE) };;
	////p3.get()->setRestitution(0.2f);
	//addObjectToWorld(std::move(p3));
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
		int steps = mDynamicsWorld->stepSimulation(mFixedTimeStep, 10);
		mAccumulator -= mFixedTimeStep;
	}

	updateObjectsTransform();
}

void PhysicsWorld::updateObjectsTransform()
{
	for (RigidBody* rb : m_rigidBodies)
	{
		Object* parentObject = rb->getParentObject();
		Transform* t = parentObject->getComponent<Transform>();
		t->setPosition(t->bulletToGlmVec3(
			rb->getRigidBody()->getCenterOfMassPosition()
		));
		t->setRotation(t->bulletToGlmQuat(
			rb->getRigidBody()->getOrientation()
		));
	}
}

void PhysicsWorld::addObjectToWorld(RigidBody* rbComponent)
{
	if (!rbComponent)
		throw std::runtime_error("RigidBody is null");
	m_rigidBodies.push_back(rbComponent);
	mDynamicsWorld->addRigidBody(rbComponent->getRigidBody());
}
