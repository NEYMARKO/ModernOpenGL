#include "Shader.h"
#include "Camera.h"
#include "Lighting.h"

	// Making 3D bounding box for quad (2D shape), for rayBoxIntersection code to work on 2D quad
	// => need to make thickness something really small, x and y values (width and height) should be 1 => [-0.5, 0.5] range - pivot in middle
Lighting::Lighting(Shader* shaderProgram, const glm::vec3& position, const glm::vec3& color) 
	: SceneEntity{ EditorCollider { glm::vec3(-0.5f, -0.5f, -THICKNESS / 2), glm::vec3(0.5f, 0.5f, THICKNESS / 2) }, Transform() },
	mShaderProgram { shaderProgram }, mColor{ color },
	m_spriteRenderer{ std::move(Texture {"light-bulb.png"}) }
{
	m_spriteRenderer.setParentObject(this);
	m_editorCollider.setParent(this);
}

Lighting::~Lighting()
{
	std::cout << "DELETED LIGHTING" << std::endl;
}

glm::vec3 Lighting::getColor()
{
	return mColor;
}

void Lighting::Draw(/*Shader& boundingBoxShaderProgram, */Camera& camera)
{
	//std::cout << "Drawing light" << std::endl;
	mShaderProgram->Activate();

	glm::mat4 model = m_transform.getModelMatrix();
	mShaderProgram->SetMat4("model", model);
	camera.generateViewProjectionMatrices(*mShaderProgram);

	mShaderProgram->SetVec3("lightColor", mColor);


	//there is no point in drawing same mesh again
	
	/*lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshContainer.meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	lightVAO.Unbind();*/

	//this->meshContainer.boundingBox->Draw(boundingBoxShaderProgram, camera);
}