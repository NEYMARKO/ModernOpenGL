#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "Component.h"

class Collider;

class RigidBody : public Component
{
private:
	Collider* m_collider;
	btScalar m_mass;
	btVector3 m_inertia;
	float m_restitution;

	std::unique_ptr<btDefaultMotionState> m_motionState;
	btRigidBody::btRigidBodyConstructionInfo m_rigidBodyCI;
	std::unique_ptr<btRigidBody> m_rigidBody;

public:
	RigidBody(float mass = 1.0f, float restitution = 0.4f);
	void finalizeRigidBody();
	btVector3 calculateInertia();
	void setRestitution(float restitution);
	btDefaultMotionState* getMotionState() { return m_motionState.get(); }
	btRigidBody* getRigidBody() { return m_rigidBody.get(); }
};