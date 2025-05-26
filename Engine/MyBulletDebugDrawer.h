#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <glm/glm.hpp>
#include <vector>

class MyBulletDebugDrawer : public btIDebugDraw
{
private:
	int m_mode;
	//size_t m_prevSize;
	std::vector<glm::vec3> m_linesPoints;
public:

	MyBulletDebugDrawer() : 
		m_mode { DBG_DrawWireframe }
	{
		m_linesPoints.reserve(200);
	}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
	{
		//std::cout << "ADDED LINE" << '\n';
		m_linesPoints.emplace_back(glm::vec3(from.getX(), from.getY(), from.getZ()));
		m_linesPoints.emplace_back(glm::vec3(to.getX(), to.getY(), to.getZ()));
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
		btScalar distance, int lifeTime, const btVector3& color) override {}
	void reportErrorWarning(const char* warningString) override {}
	void draw3dText(const btVector3& location, const char* textString) override {}
	void setDebugMode(int mode) override { m_mode = mode; }
	int getDebugMode() const override { return m_mode; }

	void resetLines() 
	{
		//m_prevSize = m_lines.size();
		m_linesPoints.clear();
		/*m_lines.reserve(m_prevSize);*/
	}

	std::vector<glm::vec3>* getLinesPoints() { return &m_linesPoints; }

};