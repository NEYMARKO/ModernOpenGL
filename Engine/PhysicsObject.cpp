#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(float mass, glm::vec3& planeNormal, glm::vec3& position, glm::quat& rotation)
	: mMass { mass }, 
	mCollisionShape { new btStaticPlaneShape(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), 0) },
	mMotionState{ 
		new btDefaultMotionState(btTransform(
		btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),
		btVector3(position.x, position.y, position.z)
	))}, 
	mRigidBodyCI {mMass, mMotionState, mCollisionShape},
	mRigidBody { new btRigidBody(mRigidBodyCI) }
{
	mCollisionShape->calculateLocalInertia(mMass, mInertia);
}

PhysicsObject::~PhysicsObject()
{
	delete mRigidBody;
	delete mMotionState;
	delete mCollisionShape;
}

btRigidBody* PhysicsObject::getRigidBody()
{
	return mRigidBody;
}