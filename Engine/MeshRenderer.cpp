#include "Camera.h"
#include "Lighting.h"
#include "Object.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	: m_mesh{ mesh }, m_material{ material }
{
	//mesh needs to be scaled to [-1,1] range
	//m_parentObject->getComponent<Transform>()->setScale(m_mesh.get()->scalingFactor);
}

void MeshRenderer::changeColor(const glm::vec3& color)
{
	m_material->setDiffuse(color);
}

//void MeshRenderer::setParent(Object* parent)
//{
//	m_parentObject = parent;
//}

void MeshRenderer::draw(Camera& camera, Lighting& lighting, Transform* transform)
{
	//std::cout << "PARENT OBJECT DOES " + std::string(m_parentObject != nullptr ? "" : "NOT") + " EXIST!" << std::endl;
	Shader* shaderProgram = m_material->getShaderProgram();

	if (!shaderProgram)
	{
		std::cout << "SHADER PROGRAM DOESN'T EXISTS" << std::endl;
		return;
	}

	Transform* transformPtr = transform ? transform : getParentObject()->getComponent<Transform>();
	glm::mat4 modelMatrix = transformPtr->getModelMatrix();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", modelMatrix);
	camera.generateViewProjectionMatrices(*shaderProgram);

	shaderProgram->SetVec3("objectColor", m_material->getDiffuse());
	shaderProgram->SetVec3("lightColor", lighting.getColor());
	shaderProgram->SetVec3("lightPos", lighting.getPosition());

	VAO* VAO = m_mesh->getVAO();

	if (!VAO)
	{
		std::cout << "VAO DOESN'T EXISTS" << std::endl;
		return;
	}

	VAO->Bind();
	//mVAO.Bind();
	//std::cout << "SHOULD CALL DRAW ELEMENTS" << std::endl;
	glDrawElements(GL_TRIANGLES, m_mesh->getIndices()->size(), GL_UNSIGNED_INT, 0);
	//std::cout << "FINISHED CALLING DRAW ELEMENTS" << std::endl;
	//mVAO.Unbind();
	VAO->Unbind();
}