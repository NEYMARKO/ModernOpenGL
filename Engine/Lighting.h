#pragma once
#include "glm/glm.hpp"
#include "VAO.h"
#include "SceneEntity.h"
#include "SpriteRenderer.h"
#include "Texture.h"

class Shader;
class Camera;


#define THICKNESS 0.01f

class Lighting : public SceneEntity
{
private:
	Shader* mShaderProgram;
	glm::vec3 mColor;
	float mSpecularStrength;
	SpriteRenderer m_spriteRenderer;
public:	
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;

	//Initializes object to "mesh" and assigns it color "lightColor"
	Lighting(Shader* shaderProgram, const glm::vec3& position, const glm::vec3& color);
	~Lighting();

	glm::vec3 getColor();
	virtual void* getComponentLow(std::type_index t) override
	{
		if (t == typeid(Transform))
			return &m_transform;
		if (t == typeid(SpriteRenderer))
			return &m_spriteRenderer;
		return nullptr;
	}
	//Calculates MVP matrices, assigns uniforms and draws mesh that represents source of light
	//Uses light shader program
	void Draw(/*Shader& boundingBoxShaderProgram, */Camera& camera);
};