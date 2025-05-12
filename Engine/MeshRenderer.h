#pragma once
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "Component.h"

class Camera;
class Lighting;
class Object;

class MeshRenderer : public Component
{
private:

	//since Object owns MeshRenderer component, we need to use raw pointer
	//to avoid circular dependency.
	//MeshRenderer is going to be deleted when Object is deleted anyways
	Object* mParentObject;
	//Shader* mShaderProgram;
	std::unique_ptr<Material> m_material;
	std::unique_ptr<Mesh> m_mesh;
public:
	MeshRenderer(Object* parentObject, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);
	void setParent(Object* parent);
	void changeColor(const glm::vec3& color);
	Mesh* getMesh() { return m_mesh.get(); }
	void draw(Camera& camera, Lighting& lighting, Transform* transform = nullptr);
};
