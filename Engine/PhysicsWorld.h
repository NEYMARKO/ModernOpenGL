#pragma once
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsObject;

class PhysicsWorld
{
private:
	btBroadphaseInterface* mBroadPhase;
	btDefaultCollisionConfiguration* mCollisionConfig;
	btCollisionDispatcher* mCollisionDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;
	btDiscreteDynamicsWorld* mDynamicsWorld;

	std::vector<std::unique_ptr<PhysicsObject>> mPhysicsObjects;

	float mLastFrame{};
	float mCurrentFrame{};
	float mDeltaTime{};
	float mAccumulator{};
	const float mFixedTimeStep{ 1 / 60.f };
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
	void addObjectToWorld(std::unique_ptr<PhysicsObject> object);
};