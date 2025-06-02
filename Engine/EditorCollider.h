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
	Object* m_parent;
	std::array<glm::vec3, 8> m_vertices;
	std::array<glm::vec3, 24> m_edges;
	//min value for each dimension
	glm::vec3 m_min;
	//max value for each dimension
	glm::vec3 m_max;
	Layer m_layer{ EC_DEFAULT };
	EditorCollider(const glm::vec3& minimums, const glm::vec3& maximums, Layer layer = EC_DEFAULT);
	void setParent(Object* parent);
	void calculateExtremes();
	void setupAABB();
	//Uses slab algorithm for ray-box(AABB) intersection
	bool intersects(const glm::vec3& start, const glm::vec3& direction);
	void setLayer(Layer layer) { m_layer = layer; }
	int getLayer() { return m_layer; }
};