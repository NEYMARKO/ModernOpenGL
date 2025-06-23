#pragma once
#include <glm/glm.hpp>
#include <vector>

class Mesh;
class Material;
class Object;
class PhysicsWorld;
class Shader;

class Ragdoll
{
private:
	std::vector<Object*> m_bones;
	glm::vec3 m_position;
	PhysicsWorld* m_physicsWorld;
	Mesh* m_mesh;
	Material* m_material;
	Shader* m_shader;
public:
	Ragdoll(const glm::vec3& position, PhysicsWorld* physicsWorld) :
		m_position{ position }, m_physicsWorld { physicsWorld }
	{
		setup();
	};
	void setup();
	void addBone(const glm::vec3& position, const glm::quat& rotation, float mass);
	std::vector<Object*>* getAllBones() { return &m_bones; };
};