#pragma once
#include "Mesh.h"
#include <unordered_map>

class Gizmos
{
	private:
		Camera* camera;
		Shader* meshShader;
		Shader* boundingBoxShader;
		Shader* pointShader;
		std::unordered_map<std::string, Ray*> raysInTheScene;
		std::vector<glm::vec3> pointsPositions;
		VBO pointsVBOBuffer;
		VAO pointsVAOBuffer;
	public:
		Gizmos(Camera* camera, Shader* meshShader, Shader* boundingBoxShader, Shader* pointShader);
		~Gizmos();
		void AddRay(std::string name, const glm::vec3& startPos, const glm::vec3& direction, int length);
		void AddPoint(const glm::vec3& position);
		void SetupPointsBuffer();
		void UpdatePoints(const std::vector<glm::vec3>& pointsPositions);
		void RenderPoints(float radius);
		void RenderLine(Ray* ray, glm::vec3& color);
		void RenderAllLines(glm::vec3& color);
		void UpdateLine(std::string& name, const glm::vec3& startPos, const glm::vec3& direction, int length);
		void RenderBoundingBox(BoundingBox* boundingBox);
};