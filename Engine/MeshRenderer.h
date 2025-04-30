#pragma once
#include <memory>

#include "Material.h"
#include "Mesh.h"

class Camera;
class Lighting;
class Object;

class MeshRenderer
{
private:

	//since Object owns MeshRenderer component, we need to use raw pointer
	//to avoid circular dependency.
	//MeshRenderer is going to be deleted when Object is deleted anyways
	Object* mParentObject;
	//Shader* mShaderProgram;
	std::unique_ptr<Material> mMaterial;
	std::unique_ptr<Mesh> mMesh;
public:
	MeshRenderer(Object* parentObject, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);
	void setParent(Object* parent);
	void changeColor(const glm::vec3& color);
	Mesh* getMesh() { return mMesh.get(); }
	void draw(Camera& camera, Lighting& lighting);
};
