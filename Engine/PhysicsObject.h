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
	float mRestitution{0.8f};
	float mRadius;
	btVector3 mPlaneNormal;
	btVector3 mPosition;
	//RIGID COMPONENTS
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
		btVector3 position,
		btQuaternion rotation,
		CollisionShapeType collisionShapeType,
		btVector3 planeNormal,
		float mass,
		float radius,
		float restitution);
	//No need for any manual cleanup, since smart pointers are being used
	~PhysicsObject();
	std::unique_ptr<btCollisionShape> setCollisionShape(CollisionShapeType shapeType);
	btVector3 calculateInertia();
	btVector3 getInertia();
	void setRestitution(float restitution);
	bool checkIfSphere();
	btDefaultMotionState* getMotionState();
	btRigidBody* getRigidBody() { return mRigidBody.get(); };
};