#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <btBulletDynamicsCommon.h>

class Mesh;
class Material;
class Object;
class PhysicsWorld;
class Shader;

class Ragdoll
{
private:
	std::vector<Object*> m_bones;
	std::vector<btHingeConstraint*> m_constraints;
	glm::vec3 m_position;
	PhysicsWorld* m_physicsWorld;
	Mesh* m_mesh;
	Material* m_material;
	Shader* m_shader;
public:
	bool m_finalized = false;
	short m_finalizedCount = 0;
	Ragdoll(const glm::vec3& position, PhysicsWorld* physicsWorld) :
		m_position{ position }, m_physicsWorld { physicsWorld }
	{
		setup();
	};
	void setup();
	void addBone(const glm::vec3& position, const glm::quat& rotation, float mass);
	void addConstraint(RigidBody* rb1, RigidBody* rb2);
	void update();
	std::vector<Object*>* getAllBones() { return &m_bones; };
	~Ragdoll();
};