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


#ifndef M_PI_2
#define M_PI_2 btScalar(1.57079632679489661923)
#endif

Ragdoll::Ragdoll(const glm::vec3& position, PhysicsWorld* physicsWorld, float scale) :
	m_physicsWorld{ physicsWorld }
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

	std::cout << "POSITION: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	btVector3 btPos = offset.getOrigin();
	std::cout << "BT POS: (" << btPos.getX() << ", " << btPos.getY() << ", " << btPos.getZ() << ")\n";
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
	/*addConstraint(m_bones[3]->getComponent<RigidBody>(), m_bones[4]->getComponent<RigidBody>());
	addConstraint(m_bones[5]->getComponent<RigidBody>(), m_bones[6]->getComponent<RigidBody>());*/
	addConstraint(m_bones[0]->getComponent<RigidBody>(), 
		m_bones[1]->getComponent<RigidBody>(), btVector3(0,0,1));
	addConstraint(m_bones[2]->getComponent<RigidBody>(), 
		m_bones[3]->getComponent<RigidBody>(), btVector3(0,0,1));
}

Ragdoll::~Ragdoll()
{
	//int i;
	//for (i = 0; i < JOINTS_COUNT; i++)
	//{
	//	if (m_jointConstraints[i])
	//	{
	//		m_physicsWorld->getDynamicsWorld()->removeConstraint(m_jointConstraints[i]);
	//		delete m_jointConstraints[i];
	//		m_jointConstraints[i] = 0;
	//	}
	//}
	//for (i = 0; i < BONES_COUNT; i++)
	//{
	//	delete m_bones[i];
	//	m_bones[i] = 0;
	//	/*delete m_shapes[i];
	//	m_shapes[i] = 0;*/
	//}
}