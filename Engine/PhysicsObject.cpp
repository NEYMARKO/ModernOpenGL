#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(
	const glm::vec3& position,
	const glm::quat& rotation,
	CollisionShape collisionShapeType,
	const glm::vec3& planeNormal,
	float mass,
	float radius,
	float restitution)
	: mMass{ mass }, mRestitution{ restitution }, mPlaneNormal{ planeNormal },
	mCollisionShape{ setCollisionShape(collisionShapeType) },
	mInertia{ calculateInertia() },
	mMotionState{
		std::make_unique<btDefaultMotionState>(btTransform(
		btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),
		btVector3(position.x, position.y, position.z)
	)) },
	mRigidBodyCI{ mMass, mMotionState.get(), mCollisionShape.get(), mInertia },
	mRigidBody{ std::make_unique<btRigidBody>(mRigidBodyCI) },
	mRadius { radius } 
{
	mRigidBody.get()->setRestitution(mRestitution);
}

PhysicsObject::~PhysicsObject()
{
	std::cout << "Deleted Physics object" << std::endl;
}

std::unique_ptr<btCollisionShape> PhysicsObject::setCollisionShape(CollisionShape shapeType)
{
	switch (shapeType)
	{
	case CollisionShape::SPHERE:
		return std::make_unique<btSphereShape>(mRadius);  // OK: returns unique_ptr<btCollisionShape>
	case CollisionShape::PLANE:
		return std::make_unique<btStaticPlaneShape>(
			btVector3(mPlaneNormal.x, mPlaneNormal.y, mPlaneNormal.z), 0
		);
	case CollisionShape::CUBE:
		//POSITION IS DEFINED IN MOTION STATE - THIS IS WHERE SHAPE DIMENSIONS
		// ARE DEFINED
		// return std::make_unique<btBoxShape>(...);
		break;
	case CollisionShape::CAPSULE:
		// return std::make_unique<btCapsuleShape>(...);
		break;
	default:
		std::cerr << "Warning: Unknown CollisionShape type! Using EmptyShape.\n";
		return std::make_unique<btEmptyShape>();
	}
}

btVector3 PhysicsObject::calculateInertia()
{
	btVector3 inertia(0, 0, 0);
	if (mMass != 0.0f)
		mCollisionShape->calculateLocalInertia(mMass, inertia);
	return inertia;
}

btVector3 PhysicsObject::getInertia()
{
	return mInertia;
}

btRigidBody* PhysicsObject::getRigidBody()
{
	return mRigidBody.get();
}

btDefaultMotionState* PhysicsObject::getMotionState()
{
	return mMotionState.get();
}

void PhysicsObject::setRestitution(float restitution)
{
	mRestitution = restitution;
	mRigidBody->setRestitution(restitution);
}