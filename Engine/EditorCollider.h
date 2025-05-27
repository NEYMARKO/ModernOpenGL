#pragma once
#include <array>
#include <glm/glm.hpp>

class Object;

enum Layer
{
	DEFAULT,
	OBJECT,
	GIZMOS
};

struct EditorCollider
{
	Object* m_parent;
	std::array<glm::vec3, 8> m_boundingPoints;
	glm::vec3 m_min;
	glm::vec3 m_max;
	Layer m_layer{ DEFAULT };
	EditorCollider(const glm::vec3& minimums, const glm::vec3& maximums, Layer layer = DEFAULT);
	void setParent(Object* parent);
	void calculateExtremes();
	void setupAABB();
	//Uses slab algorithm for ray-box(AABB) intersection
	bool intersects(const glm::vec3& start, const glm::vec3& direction);
	void setLayer(Layer layer) { m_layer = layer; }
	int getLayer() { return m_layer; }
};