#include <math.h>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Object.h"
#include "Transform.h"
#include "EditorCollider.h"
EditorCollider::EditorCollider(const glm::vec3& minimums, const glm::vec3& maximums, Layer layer) :
	m_min{ minimums }, m_max { maximums }, m_layer { layer }
{
	m_vertices =
	{
		glm::vec3(m_min.x, m_min.y, m_min.z),
		glm::vec3(m_min.x, m_min.y, m_max.z),
		glm::vec3(m_min.x, m_max.y, m_min.z),
		glm::vec3(m_min.x, m_max.y, m_max.z),
		glm::vec3(m_max.x, m_min.y, m_min.z),
		glm::vec3(m_max.x, m_min.y, m_max.z),
		glm::vec3(m_max.x, m_max.y, m_min.z),
		glm::vec3(m_max.x, m_max.y, m_max.z),
	};
	setupAABB();
}

void EditorCollider::setParent(Object* parent)
{
	m_parent = parent;
	Transform* t = m_parent->getComponent<Transform>();
	glm::vec3 pos = t->getPosition();
	//transforming them to world space
	for (auto& point : m_vertices)
	{
		point += pos;
	}
	calculateExtremes();
}
void EditorCollider::calculateExtremes()
{
	m_min = m_vertices[0];
	m_max = m_vertices[0];
	for (const auto& point : m_vertices)
	{
		m_min = glm::min(m_min, point);
		m_max = glm::max(m_max, point);
	}
}
void EditorCollider::setupAABB()
{
	calculateExtremes();
	m_vertices = 
	{
		glm::vec3(m_min.x, m_min.y, m_min.z),
		glm::vec3(m_min.x, m_min.y, m_max.z),
		glm::vec3(m_min.x, m_max.y, m_min.z),
		glm::vec3(m_min.x, m_max.y, m_max.z),
		glm::vec3(m_max.x, m_min.y, m_min.z),
		glm::vec3(m_max.x, m_min.y, m_max.z),
		glm::vec3(m_max.x, m_max.y, m_min.z),
		glm::vec3(m_max.x, m_max.y, m_max.z),
	};

	m_edges =
	{
		// Bottom square
		m_vertices[0], m_vertices[4],
		m_vertices[4], m_vertices[5],
		m_vertices[5], m_vertices[1],
		m_vertices[1], m_vertices[0],

		// Top square
		m_vertices[2], m_vertices[6],
		m_vertices[6], m_vertices[7],
		m_vertices[7], m_vertices[3],
		m_vertices[3], m_vertices[2],

		// Vertical edges
		m_vertices[0], m_vertices[2],
		m_vertices[4], m_vertices[6],
		m_vertices[5], m_vertices[7],
		m_vertices[1], m_vertices[3]
	};
}


bool EditorCollider::intersects(const glm::vec3& start, const glm::vec3& direction)
{
	float t_close[3]{0.0f};
	float t_far[3]{0.0f};
	for (int i = 0; i < 3; i++)
	{
		if (direction[i] == 0)
			//if the current dimension of direction vector is parallel to slab
			//we need to check whether origin of ray is outside of box - if it is,
			//it will never intersect with the box
		{
			if (start[i] < m_min[i] || start[i] > m_max[i])
				return false;
		}
		//avoid calculating if direction[i] == 0 to avoid division by 0
		else
		{
			float t_i_low = (m_min[i] - start[i]) / direction[i];
			float t_i_high = (m_max[i] - start[i]) / direction[i];
			t_close[i] = fmin(t_i_low, t_i_high);
			t_far[i] = fmax(t_i_low, t_i_high);
		}
	}

	float t_close_float = *std::max_element(std::begin(t_close), std::end(t_close));
	float t_far_float = *std::min_element(std::begin(t_far), std::end(t_far));

	/*std::cout << "BOUNDING VOLUME of : " << m_parent->getName() << '\n';
	for (const auto& point : m_vertices)
	{
		std::cout << glm::to_string(point) << '\n';
	}*/

	return t_close_float <= t_far_float;
}