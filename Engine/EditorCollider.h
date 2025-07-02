#pragma once
#include <array>
#include <glm/glm.hpp>

class Object;

enum Layer
{
	EC_DEFAULT,
	EC_OBJECT,
	EC_GIZMOS
};

struct EditorCollider
{
	Object* m_parent = nullptr;
	//min value for each dimension
	/*const */glm::vec3 m_min;
	//max value for each dimension
	/*const */glm::vec3 m_max;
	Layer m_layer{ EC_DEFAULT };
	const std::array<glm::vec3, 8> m_origVerts;
	std::array<glm::vec3, 8> m_vertices;
	std::array<glm::vec3, 24> m_edges;
	glm::vec3 m_colliderPosition;
	EditorCollider(const glm::vec3& minimums, const glm::vec3& maximums, Layer layer = EC_DEFAULT);
	void setParent(Object* parent);
	void setupAABB();
	void calculateExtremes();
	//Uses slab algorithm for ray-box(AABB) intersection
	bool intersects(const glm::vec3& start, const glm::vec3& direction, glm::vec3& intersectionOut);
};