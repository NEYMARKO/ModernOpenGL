#pragma once
#include "Scene.h"
#include "PhysicsWorld.h"
#include "RigidBodyRegistry.h"

class World
{
private:
	Scene m_scene;
	PhysicsWorld m_physicsWorld;
	RigidBodyRegistry m_rigidBodyRegistry;
public:
	World() = default;
	void init();
	void tick();
	//static PhysicsWorld* getPhysicsWorld() { return &m_physicsWorld; }
};
