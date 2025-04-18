#include "Mesh.h"
#include "Camera.h"
#include "Lighting.h"
#include "Object.h"
#include "Transform.h"
#include "Material.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Object* object, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material)
	: mParentObject{ object }, mMesh{ std::move(mesh) }, mMaterial{ std::move(material) }
{
}

void MeshRenderer::draw(Camera& camera, Lighting& lighting)
{
	Shader* shaderProgram = mMaterial.get()->getShaderProgram();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", mParentObject->getComponent<Transform>()->getModelMatrix());
	camera.generateViewProjectionMatrices(*shaderProgram);

	shaderProgram->SetVec3("objectColor", mMaterial.get()->getDiffuse());
	shaderProgram->SetVec3("lightColor", lighting.getColor());
	shaderProgram->SetVec3("lightPos", lighting.getPosition());

	mMesh.get()->getVAO()->Bind();
	//mVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->mMeshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	//mVAO.Unbind();
	mMesh.get()->getVAO()->Unbind();
}