#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE
#include "RigidBody.h"
#include "Transform.h"
//#include "Object.h"
#include "RigidBodyRegistry.h"
#include "PhysicsWorld.h"
#include "SceneEntity.h"

#define DEFAULT_WORLD_ID 1

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
	if (!dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld))
		throw std::runtime_error("NEED TO MODIFY CODE TO HANDLE DIFFERENT TYPES OF WORLDS");

	for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		mDynamicsWorld->removeCollisionObject(obj);
	}


	// RigidBody component is attached to Object as unique_ptr. Once Object gets destroyed, it's 
	// RigidBody will be destroyed aswell => because RigidBody component holds btDefaultMotionState
	// and btRigidBody as unique_ptr, their destruction will be handled automatically
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
}

void PhysicsWorld::fixedUpdate()
{
	handleHangingRigidBodies();
	
	handleDiscardedRigidBodies();
	
	updateDeltaTime();

	simulate();


}

void PhysicsWorld::updateDeltaTime()
{
	m_currentFrame = static_cast<float>(glfwGetTime());
	mDeltaTime = m_currentFrame - mLastFrame;
	if (mDeltaTime > 10 * mFixedTimeStep)
		mDeltaTime = 10 * mFixedTimeStep;
	mLastFrame = m_currentFrame;

}

void PhysicsWorld::simulate()
{
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
		SceneEntity* parentObject = rb->getParentObject();
		Transform* t = parentObject->getComponent<Transform>();
		t->setPosition(t->bulletToGlmVec3(
			rb->getRigidBody()->getCenterOfMassPosition()
		));
		t->setRotation(t->bulletToGlmQuat(
			rb->getRigidBody()->getOrientation()
		));
	}
}

void PhysicsWorld::handleHangingRigidBodies()
{
	//if (!RigidBodyRegistry::pendingForWorld(DEFAULT_WORLD_ID)) return;
	while (RigidBodyRegistry::pendingForWorld(DEFAULT_WORLD_ID))
	{
		RigidBody* rb = RigidBodyRegistry::getFromQueue(DEFAULT_WORLD_ID);
		if (rb)
		{
			addObjectToWorld(rb);
		}
	}
}

void PhysicsWorld::handleDiscardedRigidBodies()
{
	while (RigidBodyRegistry::pendingForDeletion(DEFAULT_WORLD_ID))
	{
		RigidBody* rb = RigidBodyRegistry::getDiscardedFromQueue(DEFAULT_WORLD_ID);
		if (rb)
		{
			removeObjectFromWorld(rb);
		}
	}
}

void PhysicsWorld::addObjectToWorld(RigidBody* rbComponent)
{
	if (!rbComponent)
		throw std::runtime_error("RigidBody is null");
	m_rigidBodies.push_back(rbComponent);
	std::cout << "RB COMPONENT ADDDED TO WORLD\n";
	rbComponent->m_addedToWorld = true;
	mDynamicsWorld->addRigidBody(rbComponent->getRigidBody());
}

void PhysicsWorld::removeObjectFromWorld(RigidBody* rbComponent)
{

	if (RigidBodyRegistry::pendingForWorld(DEFAULT_WORLD_ID))
		throw std::runtime_error("RIGID_BODY_REGISTRY NOT SYNCED WITH WORLD - some rigid_bodies might not have been added yet\n");
	if (!rbComponent)
		throw std::runtime_error("RigidBody is nullptr");

	// check if any addition is pending - check queue
	auto iterator = std::find(m_rigidBodies.begin(), m_rigidBodies.end(), rbComponent);
	if (iterator == m_rigidBodies.end())
	{
		std::cout << "RigidBody component not found\n";
		return;
	}

	int position = iterator - m_rigidBodies.begin();

	btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[position];
	if (obj && mDynamicsWorld)
	{
		// btDiscreteDynamicsWorld::removeCollisionObject() checks whether argument passed to it
		// is of type btRigidBody* or btCollisionObject* - user doesn't have to do that check himself.
		// However, if m_dynamicsWorld is of some more generic type (for example btDynamicWorld, which
		// is ancestor class to btDiscreteDynamicsWorld, it won't do that check and it may result
		// in cleaning process not being complete) 
		if (auto* casted = dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld))
			mDynamicsWorld->removeCollisionObject(obj);
		m_rigidBodies.erase(iterator);
	}
}