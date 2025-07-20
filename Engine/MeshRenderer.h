#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "Material.h"

class Mesh;
class Camera;
class Lighting;
class Object;
class Transform;

class MeshRenderer : public Component
{
private:
	Material* m_material;
	Mesh* m_mesh;
public:
	MeshRenderer(Mesh* mesh, Material* material) :
		m_mesh{ mesh }, m_material{ material }
	{}

	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(MeshRenderer)) return this;
		return nullptr;
	}
	Mesh* getMesh() { return m_mesh; }
	void changeColor(const glm::vec3& color) { m_material->setDiffuse(color); };
	void draw(Camera& camera, Lighting& lighting, Transform* transform = nullptr);
};
