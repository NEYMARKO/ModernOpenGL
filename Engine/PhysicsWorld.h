#pragma once
#include <iostream>
#include <chrono>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

class PhysicsWorld
{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* collisionDispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	float lastFrame{};
	float currentFrame{};
	float deltaTime{};
	float accumulator{};
	const float fixedTimeStep{ 1 / 60.f };
public:
	PhysicsWorld();
	//Bullet physics cleanup
	~PhysicsWorld();

	// Updates duration between current and last frame
	// In case of stuter sets delta time to max allowed delay
	void updateDeltaTime();
	// Starts dynamic's world simulation
	// In case of previous delay, simulates multiple steps until simulation catches up
	void simulate();
};