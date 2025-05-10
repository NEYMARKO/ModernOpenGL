#include "Object.h"
#include "Transform.h"
#include "Collider.h"
#include "RigidBody.h"

RigidBody::RigidBody(float mass, float restitution)
	: m_mass{ mass }, m_restitution{ restitution },
	m_rigidBodyCI{ btRigidBody::btRigidBodyConstructionInfo(m_mass, nullptr, nullptr, btVector3(0,0,0)) }
{
}

void RigidBody::finalizeRigidBody()
{
	//preventing multiple initializations
	if (m_rigidBody) return;

	Object* parentObject = getParentObject();

	if (!parentObject) throw std::runtime_error("Parent object is null");

	Transform* t = parentObject->getComponent<Transform>();
	m_collider = parentObject->getComponent<Collider>();

	if (!t || !m_collider)
		throw std::runtime_error("Transform or Collider is null");

	m_inertia = calculateInertia();
	m_motionState =
		std::make_unique<btDefaultMotionState>
		(btTransform(t->getBulletQuat(), t->getBulletPosition()));
	m_rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(
		m_mass, m_motionState.get(), m_collider->getCollisionShape(), m_inertia);
	m_rigidBody = std::make_unique<btRigidBody>(m_rigidBodyCI);
}

btVector3 RigidBody::calculateInertia()
{
	btVector3 inertia(0, 0, 0);
	if (m_mass != 0.0f)
		m_collider->getCollisionShape()->calculateLocalInertia(m_mass, inertia);
	return inertia;
}

void RigidBody::setRestitution(float restitution)
{
	m_restitution = restitution;
	m_rigidBody.get()->setRestitution(restitution);
}