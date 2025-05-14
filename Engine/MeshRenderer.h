#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Component.h"

class Material;
class Mesh;
class Camera;
class Lighting;
class Object;

class MeshRenderer : public Component
{
private:
	//Shader* mShaderProgram;
	Material* m_material;
	Mesh* m_mesh;
public:
	MeshRenderer(Mesh* mesh, Material* material);
	//void setParent(Object* parent);
	void changeColor(const glm::vec3& color);
	Mesh* getMesh() { return m_mesh; }
	void draw(Camera& camera, Lighting& lighting, Transform* transform = nullptr);
};
