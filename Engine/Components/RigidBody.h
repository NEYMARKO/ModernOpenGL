#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "./Component.h"

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
	bool m_addedToWorld = false;

	RigidBody(float mass = 1.0f, float restitution = 0.4f);
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(RigidBody)) return this;
		return nullptr;
	}
	void finalizeRigidBody();
	btVector3 calculateInertia();
	void setRestitution(float restitution);
	btDefaultMotionState* getMotionState() { return m_motionState.get(); }
	btRigidBody* getRigidBody() { return m_rigidBody.get(); }
};