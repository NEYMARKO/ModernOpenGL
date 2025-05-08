#pragma once
#include <iostream>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

enum CollisionShapeType
{
	SPHERE,
	CAPSULE,
	PLANE,
	CUBE
};

class PhysicsObject
{
private:
	btScalar mMass;
	float mRestitution;
	float mRadius;
	btVector3 m_position;
	btVector3 mPlaneNormal;
	std::unique_ptr<btCollisionShape> mCollisionShape;
	//Must be defined after collisionShape because it is using collision
	//function from collision shape to calculate inertia value
	btVector3 mInertia;
	std::unique_ptr<btDefaultMotionState> mMotionState;
	btRigidBody::btRigidBodyConstructionInfo mRigidBodyCI;
	std::unique_ptr<btRigidBody> mRigidBody;
	CollisionShapeType mCollisionShapeType{PLANE};
	//COLLISION COMPONENTS
public:
	PhysicsObject(
		float mass,
		float restitution,
		float radius,
		btVector3 position,
		btQuaternion rotation,
		btVector3 planeNormal,
		CollisionShapeType collisionShapeType
	);
	//No need for any manual cleanup, since smart pointers are being used
	~PhysicsObject();
	std::unique_ptr<btCollisionShape> setCollisionShape(CollisionShapeType shapeType);
	btVector3 calculateInertia();
	void setRestitution(float restitution);
	btDefaultMotionState* getMotionState() { return mMotionState.get(); };
	btRigidBody* getRigidBody() { return mRigidBody.get(); };
};