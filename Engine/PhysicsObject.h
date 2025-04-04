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
	btVector3 mInertia;
	float mRestitution{0.8f};
	float radius;
	glm::vec3 mPlaneNormal;
	//RIGID COMPONENTS
	std::unique_ptr<btCollisionShape> mCollisionShape;
	std::unique_ptr<btDefaultMotionState> mMotionState;
	btRigidBody::btRigidBodyConstructionInfo mRigidBodyCI;
	std::unique_ptr<btRigidBody> mRigidBody;

	CollisionShape mCollisionShapeType{PLANE};
	//COLLISION COMPONENTS
public:
	PhysicsObject(glm::vec3& position, glm::quat& rotation, CollisionShape& collisionShapeType, float mass, glm::vec3 planeNormal, float radius, float restitution);
	//No need for any manual cleanup, since smart pointers are being used
	~PhysicsObject();
	std::unique_ptr<btCollisionShape> setCollisionShape(CollisionShape& shapeType);
	btVector3 calculateInertia();
	void setRestitution(float restitution);

	btRigidBody* getRigidBody();
};