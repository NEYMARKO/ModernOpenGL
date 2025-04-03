#pragma once
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>

class PhysicsObject
{
private:
	btScalar mMass;
	btVector3 mInertia{0, 0, 0};
	float mRestitution;
	//RIGID COMPONENTS
	btCollisionShape* mCollisionShape;
	btDefaultMotionState* mMotionState;
	btRigidBody::btRigidBodyConstructionInfo mRigidBodyCI;
	btRigidBody* mRigidBody;

	//COLLISION COMPONENTS
public:
	PhysicsObject(float mass, glm::vec3& planeNormal, glm::vec3& position, glm::quat& rotation);
	~PhysicsObject();
	void calculateInertia();
	void setRestitution();

	btRigidBody* getRigidBody();
};