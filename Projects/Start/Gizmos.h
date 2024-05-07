#pragma once
#include "Mesh.h"
#include <unordered_map>
class Gizmos
{
	private:
		Camera* camera;
		Shader* meshShader;
		Shader* boundingBoxShader;
		std::unordered_map<std::string, Ray*> raysInTheScene;
	public:
		Gizmos(Camera* camera, Shader* meshShader, Shader* boundingBoxShader);
		~Gizmos();
		void AddRay(std::string name, const glm::vec3& startPos, const glm::vec3& direction, int length);
		void RenderBoundingBox(BoundingBox* boundingBox);
		void RenderDot(float radius, glm::vec3& position);
		void RenderLine(Ray* ray, glm::vec3& color);
		void RenderAllLines(glm::vec3& color);
		void UpdateLine(std::string& name, const glm::vec3& startPos, const glm::vec3& direction, int length);
};