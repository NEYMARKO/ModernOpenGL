#include "Camera.h"
#include "Lighting.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Object* object, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material)
	: mParentObject{ object }, mMesh{ std::move(mesh) }, mMaterial{ std::move(material) }
{
	//mesh needs to be scaled to [-1,1] range
	//mParentObject->getComponent<Transform>()->setScale(mMesh.get()->scalingFactor);
}

void MeshRenderer::changeColor(const glm::vec3& color)
{
	mMaterial.get()->setDiffuse(color);
}

void MeshRenderer::setParent(Object* parent)
{
	mParentObject = parent;
}

void MeshRenderer::draw(Camera& camera, Lighting& lighting)
{
	//std::cout << "PARENT OBJECT DOES " + std::string(mParentObject != nullptr ? "" : "NOT") + " EXIST!" << std::endl;
	Shader* shaderProgram = mMaterial.get()->getShaderProgram();

	if (!shaderProgram)
	{
		std::cout << "SHADER PROGRAM DOESN'T EXISTS" << std::endl;
		return;
	}

	shaderProgram->Activate();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", mParentObject->getComponent<Transform>()->getModelMatrix());
	camera.generateViewProjectionMatrices(*shaderProgram);

	shaderProgram->SetVec3("objectColor", mMaterial.get()->getDiffuse());
	shaderProgram->SetVec3("lightColor", lighting.getColor());
	shaderProgram->SetVec3("lightPos", lighting.getPosition());

	VAO* VAO = mMesh.get()->getVAO();

	if (!VAO)
	{
		std::cout << "VAO DOESN'T EXISTS" << std::endl;
		return;
	}

	VAO->Bind();
	//mVAO.Bind();
	//std::cout << "SHOULD CALL DRAW ELEMENTS" << std::endl;
	glDrawElements(GL_TRIANGLES, mMesh.get()->getIndices()->size(), GL_UNSIGNED_INT, 0);
	//std::cout << "FINISHED CALLING DRAW ELEMENTS" << std::endl;
	//mVAO.Unbind();
	VAO->Unbind();
}