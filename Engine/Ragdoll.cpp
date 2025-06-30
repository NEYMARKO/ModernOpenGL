#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "CapsuleCollider.h"
#include "PhysicsWorld.h"
#include "MeshLoader.h"
#include "Shader.h"
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
	m_physicsWorld{ physicsWorld }, m_scale{ scale }
{
	MeshLoader jointLoader("joint4.txt");
	//m_mesh = ResourceManager<Mesh>::getResource("joint");
	m_mesh = new Mesh(&jointLoader);
	m_shader = new Shader("default.vert", "default.frag");
	//if (m_mesh == nullptr) throw std::runtime_error("MESH IS NULLPTR");
	//m_material = ResourceManager<Material>::getResource("joint");
	m_material = new Material(m_shader);


	//Adding bones and shapes
	btTransform offset;

	offset.setIdentity();
	offset.setOrigin(btVector3(position.x, position.y, position.z));

	btTransform transform;
	
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
	addBone(BONE_HIPS, offset * transform, 0.15 * scale, 0.2 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
	addBone(BONE_SPINE, offset * transform, 0.15 * scale, 0.28 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
	addBone(BONE_HEAD, offset * transform, 0.1 * scale, 0.05 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.18), btScalar(.65), btScalar(0.)));
	addBone(BONE_UPPER_LEG_LEFT, offset * transform, 0.07 * scale, 0.45 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
	addBone(BONE_LOWER_LEG_LEFT, offset * transform, 0.05 * scale, 0.37 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.18), btScalar(.65), btScalar(0.)));
	addBone(BONE_UPPER_LEG_RIGHT, offset * transform, 0.07 * scale, 0.45 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
	addBone(BONE_LOWER_LEG_RIGHT, offset * transform, 0.05 * scale, 0.37 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, M_PI_2);
	addBone(BONE_UPPER_ARM_LEFT, offset * transform, 0.05 * scale, 0.33 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, M_PI_2);
	addBone(BONE_LOWER_ARM_LEFT, offset * transform, 0.04 * scale, 0.25 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
	addBone(BONE_UPPER_ARM_RIGHT, offset * transform, 0.05 * scale, 0.33 * scale);
	transform.setIdentity();
	transform.setOrigin(scale * btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
	transform.getBasis().setEulerZYX(0, 0, -M_PI_2);
	addBone(BONE_LOWER_ARM_RIGHT, offset * transform, 0.04 * scale, 0.25 * scale);


	for (int i = 0; i < BONES_COUNT; i++)
	{
		btRigidBody* rb = m_bones[i]->getComponent<RigidBody>()->getRigidBody();
		rb->setDamping(btScalar(0.05), btScalar(0.85));
		rb->setDeactivationTime(btScalar(0.8));
		rb->setSleepingThresholds(btScalar(1.6), btScalar(2.5));
	}
	//adding constraints

}

void Ragdoll::addBone(const Bones bone, const btTransform& btTransform, float capsuleRadius, float capsuleHeight, float mass)
{
	btVector3 btPos = btTransform.getOrigin();
	glm::vec3 pos = glm::vec3(btPos.getX(), btPos.getY(), btPos.getZ());
	btQuaternion btRot = btTransform.getRotation();
	glm::quat rot = glm::quat(btRot.getW(), btRot.getX(), btRot.getY(), btRot.getZ());

	auto boneRenderer = MeshRenderer(m_mesh, m_material);
	auto boneTransform = Transform(pos, rot);
	Object* obj = new Object{ std::move(boneTransform), std::move(boneRenderer) };
	//m_bones.emplace_back(std::move(boneTransform), std::move(boneRenderer));
	m_bones[bone] = obj;

	auto boneCollider = std::make_unique<CapsuleCollider>(capsuleRadius, capsuleHeight);
	m_bones[bone]->addComponent(std::move(boneCollider));
	auto boneRigidBody = std::make_unique<RigidBody>(mass, 0.2f);
	m_bones[bone]->addComponent(std::move(boneRigidBody));
}

void Ragdoll::addConstraint(RigidBody* rb1, RigidBody* rb2, const btVector3& axis)
{
	////rigid bodies aren't still added to the world or all constraints have been set up
	//if (!rb1->m_addedToWorld || !rb2->m_addedToWorld || m_finalizedCount >= m_bones.size() / 2)
	//	return;
	//

	//Object* parent1 = rb1->getParentObject();
	//Object* parent2 = rb2->getParentObject();
	//
	//Transform* t1 = parent1->getComponent<Transform>();
	//Transform* t2 = parent2->getComponent<Transform>();
	//
	//btVector3 worldPivot = 0.5 * (t1->getBulletPosition() + t2->getBulletPosition());

	//btTransform transA = rb1->getRigidBody()->getCenterOfMassTransform();
	//btTransform transB = rb2->getRigidBody()->getCenterOfMassTransform();

	//btVector3 pivotInA = transA.inverse() * worldPivot;
	//btVector3 pivotInB = transB.inverse() * worldPivot;

	//btVector3 axisInA = transA.getBasis().inverse() * axis; 
	//btVector3 axisInB = transB.getBasis().inverse() * axis;

	//btHingeConstraint* hinge = new btHingeConstraint(
	//	*rb1->getRigidBody(),
	//	*rb2->getRigidBody(),
	//	pivotInA,
	//	pivotInB,
	//	axisInA,
	//	axisInB
	//);
	//hinge->setLimit(-SIMD_PI / 2.0f, SIMD_PI / 2.0f);
	///*rb1->getRigidBody()->setDamping(0.05f, 0.85f);
	//rb2->getRigidBody()->setDamping(0.05f, 0.85f);*/
	//m_jointConstraints[] = hinge;
	///*rb1->getRigidBody()->activate(true);
	//rb2->getRigidBody()->activate(true);*/
	//m_physicsWorld->getDynamicsWorld()->addConstraint(m_jointConstraints.back(), true);
	//std::cout << "ADDED CONSTRAINT\n";
	//m_finalizedCount++;
}

void Ragdoll::update()
{
	if (m_finalized) 
		return;

	for (auto* obj : m_bones)
	{
		if (!(obj->getComponent<RigidBody>()->m_addedToWorld))
			return;
	}

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
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_HIPS]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[BONE_SPINE]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
	m_jointConstraints[JOINT_HIPS_SPINE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_HIPS_SPINE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, -M_PI_2);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.3), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[BONE_HEAD]->getComponent<RigidBody>()->getRigidBody(), localA, localB);

	coneConstraint->setLimit(M_PI_4, M_PI_4);
	m_jointConstraints[JOINT_LEFT_HIP] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_HIP], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, -M_PI_4 * 5);
	localA.setOrigin(m_scale * btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, -M_PI_4 * 5);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_HIPS]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[BONE_UPPER_LEG_LEFT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	coneConstraint->setLimit(M_PI_4, M_PI_4, 0);
	m_jointConstraints[JOINT_LEFT_HIP] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_HIP], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_LEG_LEFT]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[BONE_LOWER_LEG_LEFT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	hingeConstraint->setLimit(btScalar(0), btScalar(M_PI_2));
	m_jointConstraints[JOINT_LEFT_KNEE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_KNEE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, M_PI_4);
	localA.setOrigin(m_scale * btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_4);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_HIPS]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[BONE_UPPER_LEG_RIGHT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	coneConstraint->setLimit(M_PI_4, M_PI_4, 0);
	m_jointConstraints[JOINT_RIGHT_HIP] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_HIP], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_LEG_RIGHT]->getComponent<RigidBody>()->getRigidBody(), *m_bones[BONE_LOWER_LEG_RIGHT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	hingeConstraint->setLimit(btScalar(0), btScalar(M_PI_2));
	m_jointConstraints[JOINT_RIGHT_KNEE] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_KNEE], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, M_PI);
	localA.setOrigin(m_scale * btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE]->getComponent<RigidBody>()->getRigidBody(), 
		*m_bones[BONE_UPPER_ARM_LEFT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	coneConstraint->setLimit(M_PI_2, M_PI_2, 0);
	m_jointConstraints[JOINT_LEFT_SHOULDER] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_SHOULDER], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_ARM_LEFT]->getComponent<RigidBody>()->getRigidBody(), *m_bones[BONE_LOWER_ARM_LEFT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_2), btScalar(0));
	m_jointConstraints[JOINT_LEFT_ELBOW] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_LEFT_ELBOW], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
	coneConstraint = new btConeTwistConstraint(*m_bones[BONE_SPINE]->getComponent<RigidBody>()->getRigidBody(), *m_bones[BONE_UPPER_ARM_RIGHT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	coneConstraint->setLimit(M_PI_2, M_PI_2, 0);
	m_jointConstraints[JOINT_RIGHT_SHOULDER] = coneConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_SHOULDER], true);

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, M_PI_2, 0);
	localA.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
	localB.getBasis().setEulerZYX(0, M_PI_2, 0);
	localB.setOrigin(m_scale * btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
	hingeConstraint = new btHingeConstraint(*m_bones[BONE_UPPER_ARM_RIGHT]->getComponent<RigidBody>()->getRigidBody(), *m_bones[BONE_LOWER_ARM_RIGHT]->getComponent<RigidBody>()->getRigidBody(), localA, localB);
	hingeConstraint->setLimit(btScalar(-M_PI_2), btScalar(0));
	m_jointConstraints[JOINT_RIGHT_ELBOW] = hingeConstraint;
	world->addConstraint(m_jointConstraints[JOINT_RIGHT_ELBOW], true);

	m_finalized = true;
}

Ragdoll::~Ragdoll()
{
	//int i;
	//for (i = 0; i < JOINTS_COUNT; i++)
	//{
	//	m_physicsWorld->getDynamicsWorld()->removeConstraint(m_jointConstraints[i]);
	//	delete m_jointConstraints[i];
	//	m_jointConstraints[i] = 0;
	//}
	//for (i = 0; i < BONES_COUNT; i++)
	//{
	//	delete m_bones[i];
	//	m_bones[i] = 0;
	//	/*delete m_shapes[i];
	//	m_shapes[i] = 0;*/
	//}
}