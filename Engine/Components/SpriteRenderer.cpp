#include "./SpriteRenderer.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Lighting.h"
#include "./Transform.h"
#include "../Scene/ResourceManager.h"
#include "../Rendering/Shader.h"
#include "../Core/Object.h"

void SpriteRenderer::draw(Camera* camera, Lighting* light, Transform* transform)
{
	Shader* shaderProgram = ResourceManager<Shader>::getResource("texture");

	if (!shaderProgram)
	{
		throw std::runtime_error("Shader program is missing\n");
	}

	Transform* transformPtr = transform ? transform : getParentObject()->getComponent<Transform>();
	if (!transformPtr)
		throw std::runtime_error("Transform is missing\n");

	glm::mat4 modelMatrix = transformPtr->getModelMatrix();
	shaderProgram->Activate();
	shaderProgram->SetMat4("model", modelMatrix);
	shaderProgram->SetInt("tex0", 0);
	camera->generateViewProjectionMatrices(*shaderProgram);

	m_texture.Bind();
	VAO* VAO = m_mesh.getVAO();

	if (!VAO)
		throw std::runtime_error("VAO missing\n");
	VAO->Bind();
	//Since sprite will always be drawn on a square or rectangle shape, they will always have
	//precisely 6 indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	VAO->Unbind();
	m_texture.Unbind();
}