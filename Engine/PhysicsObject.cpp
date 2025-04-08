#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(
	float mass,
	float restitution,
	float radius,
	btVector3 position,
	btQuaternion rotation,
	btVector3 planeNormal,
	CollisionShapeType collisionShapeType
)
	: mMass{ mass }, mRestitution{ restitution }, mRadius{ radius },
	mPlaneNormal{ planeNormal }, mPosition{ position },
	mCollisionShape{ setCollisionShape(collisionShapeType) },
	mInertia{ calculateInertia() },
	mMotionState{
		std::make_unique<btDefaultMotionState>(btTransform(rotation, position)) 
	},
	mRigidBodyCI{ mMass, mMotionState.get(), mCollisionShape.get(), mInertia },
	mRigidBody{ std::make_unique<btRigidBody>(mRigidBodyCI) }
{
	setRestitution(mRestitution);
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
		//std::cout << "SPHERE RADIUS: " << mRadius << std::endl;
		return std::make_unique<btSphereShape>(mRadius);
	case CollisionShapeType::PLANE:
		//std::cout << "PLANE NORMAL: " << mPlaneNormal.getX() << " " << mPlaneNormal.getY() << " " << mPlaneNormal.getZ() << std::endl;
		return std::make_unique<btStaticPlaneShape>(mPlaneNormal, mPosition.getY());
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

void PhysicsObject::setRestitution(float restitution)
{
	mRestitution = restitution;
	mRigidBody->setRestitution(restitution);
}