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

	//hips
	addBone(glm::vec3(0.0f, 1.0f, 0.0f), glm::quat(), 10);
	//spine
	addBone(glm::vec3(0.0f, 1.3f, 0.0f), glm::quat(), 8);
	//head
	addBone(glm::vec3(0.0f, 1.8f, 0.0f), glm::quat(), 5);

	//upperArmL
	addBone(glm::vec3(-0.5f, 1.5f, 0.0f), glm::quat(), 3);
	//lowerArmL
	addBone(glm::vec3(-0.8f, 1.5f, 0.0f), glm::quat(), 2);

	//upperArmR
	addBone(glm::vec3(0.5f, 1.5f, 0.0f), glm::quat(), 2);
	//lowerArmR
	addBone(glm::vec3(0.8f, 1.5f, 0.0f), glm::quat(), 2);
	
	//upperLegL
	addBone(glm::vec3(-0.2f, 0.6f, 0.0f), glm::quat(), 6);
	//lowerLegL
	addBone(glm::vec3(-0.2f, 0.2f, 0.0f), glm::quat(), 4);
	
	//upperLegR
	addBone(glm::vec3(0.2f, 0.6f, 0.0f), glm::quat(), 6);
	//lowerLegR
	addBone(glm::vec3(0.2f, 0.2f, 0.0f), glm::quat(), 4);

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
