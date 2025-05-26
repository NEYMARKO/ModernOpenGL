#pragma once
#include "VAO.h"
#include "VBO.h"
#include "MyBulletDebugDrawer.h"

class Camera;
class PhysicsWorld;
class BulletGizmos
{
private:
	VAO m_VAO;
	VBO m_VBO;
	MyBulletDebugDrawer m_bulletDebugDrawer;
	PhysicsWorld* m_physicsWorld;
	Shader* m_shaderProgram;
public:
	BulletGizmos(PhysicsWorld* physicsWorld);
	void setupBuffers();
	void updateBufferContent();
	void renderColliders(Camera* camera);
	~BulletGizmos() = default;
};