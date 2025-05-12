#include "Camera.h"
#include "Lighting.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Object* object, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material)
	: mParentObject{ object }, m_mesh{ std::move(mesh) }, m_material{ std::move(material) }
{
	//mesh needs to be scaled to [-1,1] range
	//mParentObject->getComponent<Transform>()->setScale(m_mesh.get()->scalingFactor);
}

void MeshRenderer::changeColor(const glm::vec3& color)
{
	m_material.get()->setDiffuse(color);
}

void MeshRenderer::setParent(Object* parent)
{
	mParentObject = parent;
}

void MeshRenderer::draw(Camera& camera, Lighting& lighting, Transform* transform)
{
	//std::cout << "PARENT OBJECT DOES " + std::string(mParentObject != nullptr ? "" : "NOT") + " EXIST!" << std::endl;
	Shader* shaderProgram = m_material.get()->getShaderProgram();

	if (!shaderProgram)
	{
		std::cout << "SHADER PROGRAM DOESN'T EXISTS" << std::endl;
		return;
	}

	Transform* transformPtr = transform ? transform : mParentObject->getComponent<Transform>();
	glm::mat4 modelMatrix = transformPtr->getModelMatrix();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", modelMatrix);
	camera.generateViewProjectionMatrices(*shaderProgram);

	shaderProgram->SetVec3("objectColor", m_material.get()->getDiffuse());
	shaderProgram->SetVec3("lightColor", lighting.getColor());
	shaderProgram->SetVec3("lightPos", lighting.getPosition());

	VAO* VAO = m_mesh.get()->getVAO();

	if (!VAO)
	{
		std::cout << "VAO DOESN'T EXISTS" << std::endl;
		return;
	}

	VAO->Bind();
	//mVAO.Bind();
	//std::cout << "SHOULD CALL DRAW ELEMENTS" << std::endl;
	glDrawElements(GL_TRIANGLES, m_mesh.get()->getIndices()->size(), GL_UNSIGNED_INT, 0);
	//std::cout << "FINISHED CALLING DRAW ELEMENTS" << std::endl;
	//mVAO.Unbind();
	VAO->Unbind();
}