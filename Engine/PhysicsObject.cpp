#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(
	btVector3 position,
	btQuaternion rotation,
	CollisionShapeType collisionShapeType,
	btVector3 planeNormal,
	float mass,
	float radius,
	float restitution)
	: mMass{ mass }, mRestitution{ restitution }, mPlaneNormal{ planeNormal }, mPosition{ position },
	mCollisionShape{ setCollisionShape(collisionShapeType) },
	mInertia{ calculateInertia() },
	mMotionState{
		std::make_unique<btDefaultMotionState>(btTransform(rotation, position)) 
	},
	mRigidBodyCI{ mMass, mMotionState.get(), mCollisionShape.get(), mInertia },
	mRigidBody{ std::make_unique<btRigidBody>(mRigidBodyCI) },
	mRadius { radius } 
{
	/*mCollisionShape = setCollisionShape(collisionShapeType);
	mInertia = calculateInertia();
	mMotionState = std::make_unique<btDefaultMotionState>(btTransform(
		btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),
		btVector3(position.x, position.y, position.z)
	));
	btRigidBody::btRigidBodyConstructionInfo mRigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), mCollisionShape.get(), mInertia);
	mRigidBody = std::make_unique<btRigidBody>(mRigidBodyCI);
	mRigidBody.get()->setRestitution(mRestitution);*/
}

PhysicsObject::~PhysicsObject()
{
	std::cout << "Deleted Physics object" << std::endl;
}

std::unique_ptr<btCollisionShape> PhysicsObject::setCollisionShape(CollisionShapeType shapeType)
{
	switch (shapeType)
	{
	case CollisionShapeType::SPHERE:
		std::cout << "SPHERE RADIUS: " << mRadius << std::endl;
		return std::make_unique<btSphereShape>(mRadius);
	case CollisionShapeType::PLANE:
		std::cout << "PLANE NORMAL: " << mPlaneNormal.getX() << " " << mPlaneNormal.getY() << " " << mPlaneNormal.getZ() << std::endl;
		return std::make_unique<btStaticPlaneShape>(mPlaneNormal, -1.0f);
	default:
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

//btRigidBody* PhysicsObject::getRigidBody()
//{
//	return mRigidBody.get();
//}

btDefaultMotionState* PhysicsObject::getMotionState()
{
	return mMotionState.get();
}

void PhysicsObject::setRestitution(float restitution)
{
	mRestitution = restitution;
	mRigidBody->setRestitution(restitution);
}