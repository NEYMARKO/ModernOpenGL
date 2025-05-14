#pragma once

#include <glm/glm.hpp>

#include "Scene.h"
#include "PhysicsWorld.h"
#include "RigidBodyRegistry.h"
#include "ResourceManager.h"

class World
{
private:
	Scene m_scene;
	PhysicsWorld m_physicsWorld;
	RigidBodyRegistry m_rigidBodyRegistry;
	ResourceManager<Mesh> m_meshResourceManager;
	ResourceManager<Material> m_materialResourceManager;
	ResourceManager<Shader> m_shaderResourceManager;
public:
	const glm::vec3 mGravity{ 0.0f, -9.81f, 0.0f };
	static const glm::vec3 up;
	static const glm::vec3 forward;
	static const glm::vec3 right;

	World() = default;
	void init();
	void tick();
};

//World::up = glm::vec3(0.0f, 1.0f, 0.0f);
//World::forward