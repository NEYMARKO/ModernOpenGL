#include <math.h>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "../Core/SceneEntity.h"
#include "./Transform.h"
#include "./EditorCollider.h"

#ifndef EC_ENABLE_VISUAL_DEBUG
#define EC_ENABLE_VISUAL_DEBUG
#endif

EditorCollider::EditorCollider(const glm::vec3& minimums, const glm::vec3& maximums, Layer layer) :
	m_min{ minimums }, m_max { maximums }, m_layer { layer },
	m_origVerts
	{
		glm::vec3(m_min.x, m_min.y, m_min.z),
		glm::vec3(m_min.x, m_min.y, m_max.z),
		glm::vec3(m_min.x, m_max.y, m_min.z),
		glm::vec3(m_min.x, m_max.y, m_max.z),
		glm::vec3(m_max.x, m_min.y, m_min.z),
		glm::vec3(m_max.x, m_min.y, m_max.z),
		glm::vec3(m_max.x, m_max.y, m_min.z),
		glm::vec3(m_max.x, m_max.y, m_max.z),
	}
{

#ifdef EC_ENABLE_VISUAL_DEBUG
	m_vertices = m_origVerts;
	setupAABB();
#endif

}

void EditorCollider::setParent(SceneEntity* parent)
{
	m_parent = parent;
	Transform* t = &m_parent->m_transform;

	glm::mat4 transform = t->getModelMatrix();

	//Transform local coordinates to world space
	for (auto& point : m_vertices)
	{
		point = glm::vec3(transform * glm::vec4(point, 1.0f));
	}
	setupAABB();
}

void EditorCollider::setupAABB()
{
	//std::cout << "SETTING UP\n";
	/*std::cout << "SETTING UP BOUNDING VOLUME\n";
	std::cout << "PARENT IS NULL: " << (m_parent == nullptr ? "TRUE" : "FALSE") << "\n";*/
	//follow object
	if (m_parent)
	{
		Transform* t = &m_parent->m_transform;
		glm::mat4 transform = t->getModelMatrix();
		
		int i = 0;
		for (auto point : m_origVerts)
		{
			point = glm::vec3(transform * glm::vec4(point, 1.0f));
			m_vertices[i] = point;
			i++;
		}
	}

	//Used for visual debugging
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


bool EditorCollider::intersects(const glm::vec3& start, const glm::vec3& direction, glm::vec3& intersectionOut)
{	
	if (!m_parent)
		return false;

	glm::mat4 transform = (&m_parent->m_transform)->getModelMatrix();
	glm::vec3 _start = glm::vec3(glm::inverse(transform) * glm::vec4(start, 1.0f));
	//2nd argument in vec4 has to be 0.0f because _dir is line, not a point
	glm::vec3 _dir = glm::vec3(glm::inverse(transform) * glm::vec4(direction,0.0f));

	float t_close[3]{ 0.0f };
	float t_far[3]{ 0.0f };

	for (int i = 0; i < 3; i++)
	{
		if (_dir[i] == 0)
			//if the current dimension of direction vector is parallel to slab
			//we need to check whether origin of ray is outside of box - if it is,
			//it will never intersect with the box
		{
			if (_start[i] < m_min[i] || _start[i] > m_max[i])
				return false;
		}
		//avoid calculating if direction[i] == 0 to avoid division by 0
		else
		{
			float t_i_low = (m_min[i] - _start[i]) / _dir[i];
			float t_i_high = (m_max[i] - _start[i]) / _dir[i];
			t_close[i] = fmin(t_i_low, t_i_high);
			t_far[i] = fmax(t_i_low, t_i_high);
		}
	}

	float t_close_float = *std::max_element(std::begin(t_close), std::end(t_close));
	float t_far_float = *std::min_element(std::begin(t_far), std::end(t_far));

	//Transform intersection point back to world space (_start and _dir will give intersection in local
	//space of the object)
	intersectionOut = glm::vec3(transform * glm::vec4(_start + t_close_float * _dir, 1.0f));
	return t_close_float <= t_far_float && t_far_float > 0;

}