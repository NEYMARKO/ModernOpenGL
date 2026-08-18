#include "../Rendering/Mesh.h"
#include "../Object.h"
#include "./Transform.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Lighting.h"
#include "./MeshRenderer.h"

void MeshRenderer::draw(Camera& camera, Lighting& lighting, Transform* transform)
{
	Shader* shaderProgram = m_material->getShaderProgram();

	if (!shaderProgram)
	{
		std::cout << "SHADER PROGRAM DOESN'T EXISTS" << std::endl;
		return;
	}

	Transform* transformPtr = transform ? transform : getParentObject()->getComponent<Transform>();
	if (!transformPtr)
		throw std::runtime_error("Transform is missing");

	glm::mat4 modelMatrix = transformPtr->getModelMatrix();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", modelMatrix);
	camera.generateViewProjectionMatrices(*shaderProgram);

	shaderProgram->SetVec3("objectColor", m_material->getDiffuse());
	shaderProgram->SetVec3("lightColor", lighting.getColor());
	shaderProgram->SetVec3("lightPos", lighting.m_transform.getPosition());

	VAO* VAO = m_mesh->getVAO();

	if (!VAO)
	{
		throw std::runtime_error("VAO missing\n");
	}

	VAO->Bind();
	glDrawElements(GL_TRIANGLES, m_mesh->getIndices()->size(), GL_UNSIGNED_INT, 0);
	VAO->Unbind();
}