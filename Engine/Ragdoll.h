#pragma once
#include <glm/glm.hpp>
#include <array>
#include <btBulletDynamicsCommon.h>

class Mesh;
class Material;
class Object;
class PhysicsWorld;
class Shader;

enum Bones
{
	BONE_HIPS = 0,
	BONE_SPINE,
	BONE_HEAD,

	BONE_UPPER_LEG_LEFT,
	BONE_LOWER_LEG_LEFT,

	BONE_UPPER_LEG_RIGHT,
	BONE_LOWER_LEG_RIGHT,

	BONE_UPPER_ARM_LEFT,
	BONE_LOWER_ARM_LEFT,

	BONE_UPPER_ARM_RIGHT,
	BONE_LOWER_ARM_RIGHT,

	BONES_COUNT
};

enum JointConstraints
{
	JOINT_HIPS_SPINE= 0,
	JOINT_SPINE_HEAD,

	JOINT_LEFT_HIP,
	JOINT_LEFT_KNEE,

	JOINT_RIGHT_HIP,
	JOINT_RIGHT_KNEE,

	JOINT_LEFT_SHOULDER,
	JOINT_LEFT_ELBOW,

	JOINT_RIGHT_SHOULDER,
	JOINT_RIGHT_ELBOW,

	JOINTS_COUNT
};

class Ragdoll
{
private:
	std::array<Object*, BONES_COUNT> m_bones;
	//std::array<btCollisionShape*, BONES_COUNT> m_shapes;
	std::array<btTypedConstraint*, JOINTS_COUNT> m_jointConstraints;
	glm::vec3 m_position;
	PhysicsWorld* m_physicsWorld;
	Mesh* m_mesh;
	Material* m_material;
	Shader* m_shader;
	float m_scale;
public:
	bool m_finalized = false;
	short m_finalizedCount = 0;
	Ragdoll(const glm::vec3& position, PhysicsWorld* physicsWorld, float scale = 1.0f);
	void addBone(const Bones bone, const btTransform& btTransform, float capsuleRadius, float capsuleHeight, float mass = 1.0f);
	void addConstraint(RigidBody* rb1, RigidBody* rb2, const btVector3& axis);
	void update();
	std::array<Object*, BONES_COUNT>* getAllBones() { return &m_bones; };
	~Ragdoll();
};