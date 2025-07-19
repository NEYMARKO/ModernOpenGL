#pragma once

#include <array>
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"
#include "Vertex.h"

class Camera;
class Lighting;
class Transform;

class SpriteRenderer : public Component
{
private:
	Texture m_texture;
	Mesh m_mesh;

public:
	SpriteRenderer(Texture&& texture) :
		m_texture{ texture }, 
		m_mesh{ Mesh 
			{
				std::move(std::array<VertexTex, 4>
				{{
					{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},
					{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f)},
					{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f)},
					{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)}
				}}
				),
				std::move(
					std::array<unsigned int, 6>
					{
						0, 2, 1,
						0, 3, 2
					}
				)
			}	
		}
	{}
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(SpriteRenderer)) return this;
		return nullptr;
	}
	void draw(Camera* camera, Lighting* light, Transform* transform = nullptr);
};