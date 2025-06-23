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

void Ragdoll::setup()
{
	MeshLoader jointLoader("joint4.txt");
	//m_mesh = ResourceManager<Mesh>::getResource("joint");
	m_mesh = new Mesh(&jointLoader);
	m_shader = new Shader("default.vert", "default.frag");
	//if (m_mesh == nullptr) throw std::runtime_error("MESH IS NULLPTR");
	//m_material = ResourceManager<Material>::getResource("joint");
	m_material = new Material(m_shader);

	////hips
	//addBone(glm::vec3(0.0f, 1.0f, 0.0f), glm::quat(), 10);
	////spine
	//addBone(glm::vec3(0.0f, 1.3f, 0.0f), glm::quat(), 8);
	////head
	//addBone(glm::vec3(0.0f, 1.8f, 0.0f), glm::quat(), 5);

	//upperArmL
	addBone(glm::vec3(-0.5f, 1.5f, 0.0f), 
		glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)), 3);
	//lowerArmL
	addBone(glm::vec3(-0.8f, 1.5f, 0.0f), glm::quat(), 2);

	//upperArmR
	addBone(glm::vec3(0.5f, 1.5f, 0.0f), glm::quat(), 2);
	//lowerArmR
	addBone(glm::vec3(0.8f, 1.5f, 0.0f), 
		glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 0, 1)), 2);
	
	////upperLegL
	//addBone(glm::vec3(-0.2f, 0.6f, 0.0f), glm::quat(), 6);
	////lowerLegL
	//addBone(glm::vec3(-0.2f, 0.2f, 0.0f), glm::quat(), 4);
	//
	////upperLegR
	//addBone(glm::vec3(0.2f, 0.6f, 0.0f), glm::quat(), 6);
	////lowerLegR
	//addBone(glm::vec3(0.2f, 0.2f, 0.0f), glm::quat(), 4);

	//btVector3 worldPivot = 0.5f * (m_bones[3]->getComponent<Transform>()->getBulletPosition() +
	//	m_bones[4]->getComponent<Transform>()->getBulletPosition());

	//btTransform transA = m_bones[3]->getComponent<RigidBody>()->getRigidBody()->getCenterOfMassTransform();
	//btTransform transB = m_bones[4]->getComponent<RigidBody>()->getRigidBody()->getCenterOfMassTransform();
	//
	//btVector3 pivotInA = transA.inverse() * worldPivot;
	//btVector3 pivotInB = transB.inverse() * worldPivot;

	//btVector3 axisInA = transA.getBasis().inverse() * btVector3(1, 0, 0); // hinge around x
	//btVector3 axisInB = transB.getBasis().inverse() * btVector3(1, 0, 0);

	//btHingeConstraint* hinge = new btHingeConstraint(
	//	*m_bones[3]->getComponent<RigidBody>()->getRigidBody(),
	//	*m_bones[4]->getComponent<RigidBody>()->getRigidBody(),
	//	pivotInA, pivotInB,
	//	axisInA, axisInB
	//);
	//m_constraints.push_back(hinge);
	//m_physicsWorld->getDynamicsWorld()->addConstraint(m_constraints.back());
	/*btHingeConstraint* testHinge = new btHingeConstraint(
		*m_bones[3]->getComponent<RigidBody>()->getRigidBody(),
			*m_bones[4]->getComponent<RigidBody>()->getRigidBody(),
		btVector3(0, -0.5f, 0),
		btVector3(0, 0.5f, 0),
		btVector3(1, 0, 0),
		btVector3(1, 0, 0)
	);
	m_constraints.push_back(testHinge);*/
	//m_physicsWorld->getDynamicsWorld()->addConstraint(m_constraints.back());
	//m_physicsWorld->getDynamicsWorld()->addConstraint(testHinge);
	/*m_bones[3]->getComponent<RigidBody>()->getRigidBody()->activate(true);
	m_bones[4]->getComponent<RigidBody>()->getRigidBody()->activate(true);*/
	/*btHingeConstraint* hinge2 = new btHingeConstraint(
		*m_bones[5]->getComponent<RigidBody>()->getRigidBody(),
		*m_bones[6]->getComponent<RigidBody>()->getRigidBody(),
		m_bones[5]->getComponent<Transform>()->getBulletPosition(),
		m_bones[6]->getComponent<Transform>()->getBulletPosition(),
		btVector3(0, 1, 0), btVector3(1, 0, 0)
	);
	m_constraints.push_back(hinge2);
	m_physicsWorld->getDynamicsWorld()->addConstraint(m_constraints.back());*/
	
	//RigidBody* a = m_bones[3]->getComponent<RigidBody>();
	//RigidBody* b = m_bones[4]->getComponent<RigidBody>();
	//btRigidBody* rbA = a->getRigidBody();
	//btRigidBody* rbB = b->getRigidBody();
	///*std::cout << "1ST FINALIZED: " << (a->m_finalized ? "TRUE" : "FALSE") << "2ND FINALIZED"
	//	<< (b->m_finalized ? "TRUE\n" : "FALSE\n");*/
	//btVector3 worldPivot = 0.5f * (rbA->getCenterOfMassPosition() + rbB->getCenterOfMassPosition());
	//btVector3 worldAxis(1, 0, 0); // hinge around x

	//btTransform localA, localB;
	//localA = rbA->getCenterOfMassTransform().inverse() * btTransform(btQuaternion::getIdentity(), worldPivot);
	//localB = rbB->getCenterOfMassTransform().inverse() * btTransform(btQuaternion::getIdentity(), worldPivot);

	//btHingeConstraint* hinge = new btHingeConstraint(
	//	*rbA, *rbB, localA, localB
	//);

	//hinge->setLimit(-SIMD_HALF_PI / 2, SIMD_HALF_PI / 2);
	//hinge->setDbgDrawSize(1.0f);

	//m_physicsWorld->getDynamicsWorld()->addConstraint(hinge);
	//addConstraint(m_bones[3]->getComponent<RigidBody>(), m_bones[4]->getComponent<RigidBody>());
}


void Ragdoll::addBone(const glm::vec3& position, const glm::quat& rotation, float mass)
{
	auto boneRenderer = MeshRenderer(m_mesh, m_material);
	auto boneTransform = Transform(m_position + position, rotation, glm::vec3(1.0f, 1.0f, 1.0f));
	Object* obj = new Object{ std::move(boneTransform), std::move(boneRenderer) };
	//m_bones.emplace_back(std::move(boneTransform), std::move(boneRenderer));
	m_bones.emplace_back(std::move(obj));

	auto boneCollider = std::make_unique<CapsuleCollider>(0.2f, 1.0f);
	m_bones.back()->addComponent(std::move(boneCollider));
	auto boneRigidBody = std::make_unique<RigidBody>(mass, 0.2f);
	m_bones.back()->addComponent(std::move(boneRigidBody));
}

void Ragdoll::addConstraint(RigidBody* rb1, RigidBody* rb2)
{
	if (!rb1->m_addedToWorld || !rb2->m_addedToWorld || m_finalizedCount >= m_bones.size() / 2)
		return;
	

	Object* parent1 = rb1->getParentObject();
	Object* parent2 = rb2->getParentObject();
	
	Transform* t1 = parent1->getComponent<Transform>();
	Transform* t2 = parent2->getComponent<Transform>();
	
	btVector3 worldPivot = 0.5 * (t1->getBulletPosition() + t2->getBulletPosition());

	btTransform transA = rb1->getRigidBody()->getCenterOfMassTransform();
	btTransform transB = rb2->getRigidBody()->getCenterOfMassTransform();

	btVector3 pivotInA = transA.inverse() * (worldPivot - btVector3(0, 0.5f, 0));
	btVector3 pivotInB = transB.inverse() * (worldPivot + btVector3(0, 0.5f, 0));

	btVector3 axisInA = transA.getBasis().inverse() * btVector3(1, 0, 0); // hinge around x
	btVector3 axisInB = transB.getBasis().inverse() * btVector3(1, 0, 0);

	btHingeConstraint* hinge = new btHingeConstraint(
		*rb1->getRigidBody(),
		*rb2->getRigidBody(),
		pivotInA,
		pivotInB,
		axisInA,
		axisInB
	);
	m_constraints.push_back(hinge);
	m_physicsWorld->getDynamicsWorld()->addConstraint(m_constraints.back());
	std::cout << "ADDED CONSTRAINT\n";
	m_finalizedCount++;
}

void Ragdoll::update()
{
	/*addConstraint(m_bones[3]->getComponent<RigidBody>(), m_bones[4]->getComponent<RigidBody>());
	addConstraint(m_bones[5]->getComponent<RigidBody>(), m_bones[6]->getComponent<RigidBody>());*/
	addConstraint(m_bones[0]->getComponent<RigidBody>(), m_bones[1]->getComponent<RigidBody>());
	addConstraint(m_bones[2]->getComponent<RigidBody>(), m_bones[3]->getComponent<RigidBody>());
}

Ragdoll::~Ragdoll()
{
	/*for (int i = m_bones.size() - 1; i > 0; i--)
	{
		delete m_bones[i];
	}*/
	/*for (int i = m_constraints.size() - 1; i > 0; i++)
	{
		delete m_constraints[i];
	}*/
}