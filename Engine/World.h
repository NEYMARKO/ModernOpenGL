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
	ResourceManager m_resourceManager;
public:
	const glm::vec3 mGravity{ 0.0f, -9.81f, 0.0f };
	const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	const glm::vec3 forward{ 0.0f, 0.0f, -1.0f };
	const glm::vec3 right{ 1.0f, 0.0f, 0.0f };

	World() = default;
	void init();
	void tick();
};
