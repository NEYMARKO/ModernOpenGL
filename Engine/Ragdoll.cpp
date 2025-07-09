#include "PhysicsWorld.h"
#include "Ragdoll.h"

#ifndef M_PI
#define M_PI btScalar(3.14159265358979323846)
#endif

#ifndef M_PI_2
#define M_PI_2 btScalar(1.57079632679489661923)
#endif

#ifndef M_PI_4
#define M_PI_4 btScalar(0.785398163397448309616)
#endif

Ragdoll::Ragdoll(const glm::vec3& position, PhysicsWorld* physicsWorld, float scale) :
	m_position{ position }, m_physicsWorld { physicsWorld }, m_scale{ scale }
{
	// Setup for geometry
	m_collisionShapes[BONE_HIPS] = new btCapsuleShape(0.15 * scale, 0.2 * scale);
	m_collisionShapes[BONE_SPINE] = new btCapsuleShape(0.15 * scale, 0.28 * scale);
	m_collisionShapes[BONE_HEAD] = new btCapsuleShape(0.1 * scale, 0.05 * scale);
	m_collisionShapes[BONE_UPPER_LEG_LEFT] = new btCapsuleShape(0.07 * scale, 0.45 * scale);
	m_collisionShapes[BONE_LOWER_LEG_LEFT] = new btCapsuleShape(0.05 * scale, 0.37 * scale);
	m_collisionShapes[BONE_UPPER_LEG_RIGHT] = new btCapsuleShape(0.07 * scale, 0.45 * scale);
	m_collisionShapes[BONE_LOWER_LEG_RIGHT] = new btCapsuleShape(0.05 * scale, 0.37 * scale);
	m_collisionShapes[BONE_UPPER_ARM_LEFT] = new btCapsuleShape(0.05 * scale, 0.33 * scale);
	m_collisionShapes[BONE_LOWER_ARM_LEFT] = new btCapsuleShape(0.04 * scale, 0.25 * scale);
	m_collisionShapes[BONE_UPPER_ARM_RIGHT] = new btCapsuleShape(0.05 * scale, 0.33 * scale);
	m_collisionShapes[BONE_LOWER_ARM_RIGHT] = new btCapsuleShape(0.04 * scale, 0.25 * scale);


	//Adding bones and shapes
	btTransform offset;

	offset.setIdentity();
	offset.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));

	btTransform transform;
	
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
	m_bones[BONE_HIPS] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_HIPS]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
	m_bones[BONE_SPINE] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_SPINE]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
	m_bones[BONE_HEAD] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_HEAD]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.18), btScalar(.65), btScalar(0.)));
	m_bones[BONE_UPPER_LEG_LEFT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_UPPER_LEG_LEFT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
	m_bones[BONE_LOWER_LEG_LEFT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_LOWER_LEG_LEFT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.18), btScalar(.65), btScalar(0.)));
	m_bones[BONE_UPPER_LEG_RIGHT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_UPPER_LEG_RIGHT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
	m_bones[BONE_LOWER_LEG_RIGHT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_LOWER_LEG_RIGHT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, M_PI_2);
	m_bones[BONE_UPPER_ARM_LEFT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_UPPER_ARM_LEFT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, M_PI_2);
	m_bones[BONE_LOWER_ARM_LEFT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_LOWER_ARM_LEFT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
	m_bones[BONE_UPPER_ARM_RIGHT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_UPPER_ARM_RIGHT]);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
	m_bones[BONE_LOWER_ARM_RIGHT] = createRigidBody(1., offset * transform, m_collisionShapes[BONE_LOWER_ARM_RIGHT]);


	for (int i = 0; i < BONES_COUNT; i++)
	{
		btRigidBody* rb = m_bones[i];
		rb->setDamping(btScalar(0.05), btScalar(0.85));
		rb->setDeactivationTime(btScalar(0.8));
		rb->setSleepingThresholds(btScalar(1.6), btScalar(2.5));
	}

	//adding constraints
	btHingeConstraint* hingeConstraint;
	btConeTwistConstraint* coneConstraint;
	
	btDynamicsWorld* world = m_physicsWorld->getDynamicsWorld();
	btTransform localA, localB;

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_HIPS],
		*m_bones[BONE_SPINE], localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
	m_jointConstraints[JOINT_HIPS_SPINE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_HIPS_SPINE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, -M_PI_2);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.3), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE],
		*m_bones[BONE_HEAD], localA, localB);

	coneConstraint->setLimit(M_PI_4, M_PI_4);
	m_jointConstraints[JOINT_SPINE_HEAD] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_SPINE_HEAD], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, -M_PI_4 * 5);
	localA.setOrigin(m_scale * btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, -M_PI_4 * 5);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_HIPS],
		*m_bones[BONE_UPPER_LEG_LEFT], localA, localB);
	coneConstraint->setLimit(M_PI_4, M_PI_4, 0);
	m_jointConstraints[JOINT_LEFT_HIP] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_HIP], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_LEG_LEFT],
		*m_bones[BONE_LOWER_LEG_LEFT], localA, localB);
	hingeConstraint->setLimit(btScalar(0), btScalar(M_PI_2));
	m_jointConstraints[JOINT_LEFT_KNEE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_KNEE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, M_PI_4);
	localA.setOrigin(m_scale * btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_4);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_HIPS],
		*m_bones[BONE_UPPER_LEG_RIGHT], localA, localB);
	coneConstraint->setLimit(M_PI_4, M_PI_4, 0);
	m_jointConstraints[JOINT_RIGHT_HIP] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_HIP], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_LEG_RIGHT], *m_bones[BONE_LOWER_LEG_RIGHT], localA, localB);
	hingeConstraint->setLimit(btScalar(0), btScalar(M_PI_2));
	m_jointConstraints[JOINT_RIGHT_KNEE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_KNEE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, M_PI);
	localA.setOrigin(m_scale * btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE],
		*m_bones[BONE_UPPER_ARM_LEFT], localA, localB);
	coneConstraint->setLimit(M_PI_2, M_PI_2, 0);
	m_jointConstraints[JOINT_LEFT_SHOULDER] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_SHOULDER], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_ARM_LEFT], *m_bones[BONE_LOWER_ARM_LEFT], localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_2), btScalar(0));
	m_jointConstraints[JOINT_LEFT_ELBOW] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_ELBOW], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE], *m_bones[BONE_UPPER_ARM_RIGHT], localA, localB);
	coneConstraint->setLimit(M_PI_2, M_PI_2, 0);
	m_jointConstraints[JOINT_RIGHT_SHOULDER] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_SHOULDER], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_ARM_RIGHT], *m_bones[BONE_LOWER_ARM_RIGHT], localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_2), btScalar(0));
	m_jointConstraints[JOINT_RIGHT_ELBOW] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_ELBOW], true);

}

btRigidBody* Ragdoll::createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.0f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	m_physicsWorld->getDynamicsWorld()->addRigidBody(body);
	return body;
}

Ragdoll::~Ragdoll()
{
	int i;
	btDiscreteDynamicsWorld* world = m_physicsWorld->getDynamicsWorld();
	// Remove all constraints
	for (i = 0; i < JOINTS_COUNT; i++)
	{
		world->removeConstraint(m_jointConstraints[i]);
		delete m_jointConstraints[i];
		m_jointConstraints[i] = 0;
	}

	// Remove all bodies and shapes
	for (i = 0; i < BONES_COUNT; i++)
	{
		world->removeRigidBody(m_bones[i]);

		delete m_bones[i]->getMotionState();
		delete m_bones[i];
		m_bones[i] = 0;

		delete m_collisionShapes[i];
		m_collisionShapes[i] = 0;
	}
}