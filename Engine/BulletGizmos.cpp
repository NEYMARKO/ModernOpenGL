#include <glm/glm.hpp>
#include "Camera.h"
#include "PhysicsWorld.h"
#include "ResourceManager.h"
#include "BulletGizmos.h"

BulletGizmos::BulletGizmos(PhysicsWorld* physicsWorld) :
	m_physicsWorld { physicsWorld }
{
	if (!m_physicsWorld)
		throw std::runtime_error("PHYSICS WORLD NOT DEFINED!");
	m_physicsWorld->getDynamicsWorld()->setDebugDrawer(&m_bulletDebugDrawer);
	m_bulletDebugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	setupBuffers();
}

void BulletGizmos::setupBuffers()
{
	size_t maxCapacity = 100 * 500 * sizeof(glm::vec3);
	
	m_VAO.Bind();
	m_VBO = VBO(maxCapacity);

	m_VAO.LinkVBO(m_VBO, 0, 3, sizeof(glm::vec3), 0);

	m_VAO.Unbind();
	m_VBO.Unbind();
}

void BulletGizmos::updateBufferContent()
{
	if (!m_physicsWorld)
		return;
	m_bulletDebugDrawer.resetLines();
	m_physicsWorld->getDynamicsWorld()->debugDrawWorld();
	const std::vector<glm::vec3>& lines = *m_bulletDebugDrawer.getLinesPoints();
	if (lines.size() == 0) return;
	//VBO's can't handle store more information - new VBO with more capacity has to be
	//initialized
	if ((m_VBO.m_maxCapacity / sizeof(glm::vec3)) < lines.size())
	{
		m_VBO.Delete();
		m_VBO = VBO(1.5 * m_VBO.m_maxCapacity);

		m_VAO.LinkVBO(m_VBO, 0, 3, sizeof(glm::vec3), 0);

		m_VAO.Unbind();
		m_VBO.Unbind();
		std::cout << "RESIZED VBO" << '\n';
	}
	else
	{
		m_VBO.Bind();
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(ptr, lines.data(), lines.size() * sizeof(glm::vec3));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		m_VBO.Unbind();
	}
	//VBO has already been fed this information. It can get deleted
}

void BulletGizmos::renderColliders(Camera* camera)
{
	
	if (!m_shaderProgram)
		m_shaderProgram = ResourceManager<Shader>::getResource("gizmos");
	//it will call overriden version of drawLine function (MyBulletDebugDrawer::drawLine)
	//which will generate vector with all line points
	
	m_shaderProgram->Activate();
	camera->generateViewProjectionMatrices(*m_shaderProgram);

	m_VAO.Bind();
	glDrawArrays(GL_LINES, 0, m_bulletDebugDrawer.getLinesPoints()->size());
	m_VAO.Unbind();
}