#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3& position, glm::quat& rotation, CollisionShape& collisionShapeType, float mass = 0.0f, glm::vec3 planeNormal = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 0.0f, float restitution = 0.0f)
	: mMass { mass }, mInertia { calculateInertia() }, mRestitution{restitution}, mPlaneNormal{planeNormal},
	mCollisionShape { setCollisionShape(collisionShapeType)},
	mMotionState{ 
		std::make_unique<btDefaultMotionState>(btTransform(
		btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),
		btVector3(position.x, position.y, position.z)
	))}, 
	mRigidBodyCI {mMass, mMotionState.get(), mCollisionShape.get(), mInertia},
	mRigidBody { std::make_unique<btRigidBody>(mRigidBodyCI) }
{
	mRigidBody.get()->setRestitution(mRestitution);
}

PhysicsObject::~PhysicsObject()
{
	std::cout << "Deleted Physics object" << std::endl;
}

std::unique_ptr<btCollisionShape> PhysicsObject::setCollisionShape(CollisionShape& shapeType)
{
	switch (shapeType)
	{
	case CollisionShape::SPHERE:
		return std::make_unique<btSphereShape>(radius);  // OK: returns unique_ptr<btCollisionShape>
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
		return std::make_unique<btEmptyShape>();
	}
	return nullptr; // fallback if no case matched
}

btVector3 PhysicsObject::calculateInertia()
{
	btVector3 inertia;
	if (mMass != 0.0f)
		mCollisionShape->calculateLocalInertia(mMass, inertia);
	return inertia;
}

btRigidBody* PhysicsObject::getRigidBody()
{
	return mRigidBody.get();
}

void PhysicsObject::setRestitution(float restitution)
{
	mRestitution = restitution;
	mRigidBody->setRestitution(restitution);
}