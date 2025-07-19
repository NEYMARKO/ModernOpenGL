#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

struct VertexTex
{
	glm::vec3 position;
	glm::vec2 texture;
};