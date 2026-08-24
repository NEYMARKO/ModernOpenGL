#pragma once
#include "glm/glm.hpp"
#include "../Buffers/VAO.h"
#include "../Core/SceneEntity.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Transform.h"
#include "./Texture.h"

class Shader;
class Camera;


#define THICKNESS 0.01f

class Lighting : public SceneEntity
{
private:
	Shader* mShaderProgram;
	glm::vec3 mColor;
	float mSpecularStrength{ 1.0f };
	SpriteRenderer m_spriteRenderer;
public:	
	//since VAO has default constructor, lightVAO is already
	//initialized here. VERTEX_ARRAY_BUFFER has been generated
	VAO lightVAO;

	Lighting(Shader* shaderProgram, const glm::vec3& position, const glm::vec3& color);
	~Lighting();

	glm::vec3 getColor() { return mColor; };

	virtual void* getComponentLow(std::type_index t) override
	{
		if (t == typeid(Transform))
			return &m_transform;
		if (t == typeid(SpriteRenderer))
			return &m_spriteRenderer;
		return nullptr;
	}
	void Draw(Camera& camera);
};