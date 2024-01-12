#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Mesh;


struct Plane
{
	glm::vec3 normal;
	float D;
};
class BoundingBox
{
	private:
		Mesh& parentMesh;
		VBO boxVBO;
		EBO boxEBO;
		VAO boxVAO;
		glm::vec3 minExtremes;
		glm::vec3 maxExtremes;
	public:
		//std::vector<float> bounds;
		std::vector<Plane> planeCoefficients;
		//After filling VBO with info, vertices are converted to world space because they are no longer needed in object space
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> trisIndices;
		//Constructor for BoundingBox 
		//Uses extremes of Mesh to create box which contains all vertices of Mesh object
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh);
		~BoundingBox();
		
		//Initializes borders, fills vertices and indices with data 
		void Initialize();
		//Sets up VAO, VBO and EBO buffers
		void SetupBuffers();

		void VerticesToWorld();
		Plane CalculatePlaneCoefficients(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
		void UpdatePlanes();
		//Checks if ray intersects with bounding box in world space
		bool Intersects(Camera& camera, float step);
		//Used for updating BoundingBox bounds when Mesh is transformed
		void UpdateBounds(const glm::mat4& transformationMatrix);
		//Draws BoundingBox around Mesh
		void Draw(Shader& shaderProgram, Camera& camera);
};