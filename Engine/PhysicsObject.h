#pragma once
#include <iostream>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

enum CollisionShape
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
	glm::vec3 mPlaneNormal;
	//RIGID COMPONENTS
	std::unique_ptr<btCollisionShape> mCollisionShape;
	//Must be defined after collisionShape because it is using collision
	//function from collision shape to calculate inertia value
	btVector3 mInertia;
	std::unique_ptr<btDefaultMotionState> mMotionState;
	btRigidBody::btRigidBodyConstructionInfo mRigidBodyCI;
	std::unique_ptr<btRigidBody> mRigidBody;
	CollisionShape mCollisionShapeType{PLANE};
	//COLLISION COMPONENTS
public:
	PhysicsObject(const glm::vec3& position,
		const glm::quat& rotation,
		CollisionShape collisionShapeType,
		const glm::vec3& planeNormal = glm::vec3(0.0f, 0.0f, 0.0f),
		float mass = 0.0f,
		float radius = 0.0f,
		float restitution = 0.0f);
	//No need for any manual cleanup, since smart pointers are being used
	~PhysicsObject();
	std::unique_ptr<btCollisionShape> setCollisionShape(CollisionShape shapeType);
	btVector3 calculateInertia();
	btVector3 getInertia();
	void setRestitution(float restitution);
	btDefaultMotionState* getMotionState();
	btRigidBody* getRigidBody();
};