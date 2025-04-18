#pragma once
#include <memory>

class Mesh;
class Camera;
class Lighting;
class Object;
class Material;

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
	void changeColor(const glm::vec3& color);
	void draw(Camera& camera, Lighting& lighting);
};
