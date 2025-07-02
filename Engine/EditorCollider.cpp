#include <math.h>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Object.h"
#include "Transform.h"
#include "EditorCollider.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include <glm/gtx/string_cast.hpp>

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
	m_vertices = m_origVerts;
	setupAABB();
}

void EditorCollider::setParent(Object* parent)
{
	m_parent = parent;
	Transform* t = m_parent->getComponent<Transform>();
	m_colliderPosition = t->getPosition();
	glm::quat rot = t->getQuaternionRotation();

	glm::mat4 transform = t->getModelMatrix();

	float scale = m_parent->getComponent<MeshRenderer>()->getMesh()->scalingFactor;
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1 / scale));

	transform = transform * scaleMat;
	//transforming them to world space
	for (auto& point : m_vertices)
	{
		/*glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_colliderPosition);
		point = glm::vec3(translationMatrix * glm::vec4(point, 1.0f));
		point = rot * (point - m_colliderPosition) + m_colliderPosition;*/
		point = glm::vec3(transform * glm::vec4(point, 1.0f));
	}
	setupAABB();
	//calculateExtremes();
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

	//follow object
	if (m_parent)
	{
		Transform* t = m_parent->getComponent<Transform>();
		float scale = m_parent->getComponent<MeshRenderer>()->getMesh()->scalingFactor;
		//glm::vec3 scale = t->getScale();
		glm::vec3 objectPos = t->getPosition();
		glm::quat rotation = t->getQuaternionRotation();

		int i = 0;
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), objectPos);

		glm::mat4 transform = t->getModelMatrix();
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1/scale));

		transform = transform * scaleMat;
		for (auto point : m_origVerts)
		{
			//glm::mat4 rotationMatrix = glm::toMat4(rotation);
			/*point.x *= scale.x;
			point.y *= scale.y;
			point.z *= scale.z;*/
			/*point = glm::vec3(translationMatrix * glm::vec4(point, 1.0f));
			point = rotation * (point - objectPos) + objectPos;*/
			point = glm::vec3(transform * glm::vec4(point, 1.0f));
			m_vertices[i] = point;
			i++;
		}
		m_colliderPosition = objectPos;
	}

	/*calculateExtremes();
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
	};*/
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

	//std::cout << "START: " << glm::to_string(start) << ", DIR: " << glm::to_string(direction) << "\n";
	/*glm::vec3 s = glm::vec3(-4.997088f, 4.965670f, 14.800200f);
	glm::vec3 d = glm::vec3(0.014362f, -0.169323f, -0.985456f);*/

	/*glm::vec3 _start = s;
	glm::vec3 _dir = d;*/
	
	if (!m_parent)
		return false;

	float scale = m_parent->getComponent<MeshRenderer>()->getMesh()->scalingFactor;
	glm::mat4 transform = m_parent->getComponent<Transform>()->getModelMatrix();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1 / scale));
	transform = transform * scaleMat;
	glm::vec3 _start = glm::vec3(glm::inverse(transform) * glm::vec4(start, 1.0f));
	//glm::mat3 invRot = glm::mat3(transformInverse);
	glm::vec3 _dir = glm::vec3(glm::inverse(transform) * glm::vec4(direction,0.0f));

	//std::cout << "TRANSFORM: " << glm::to_string(transform) << "\n";

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

	intersectionOut = glm::vec3(transform * glm::vec4(_start + t_close_float * _dir, 1.0f));
	return t_close_float <= t_far_float && t_far_float > 0;

}