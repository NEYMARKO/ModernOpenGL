#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Mesh;

class BoundingBox
{
	private:
		Mesh& parentMesh;
		VBO boxVBO;
		EBO boxEBO;
		VAO boxVAO;
	public:
		std::vector<float> bounds;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		//Constructor for BoundingBox 
		//Uses extremes of Mesh to create box which contains all vertices of Mesh object
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh);
		~BoundingBox();
		
		//Initializes borders, fills vertices and indices with data 
		void Initialize();
		//Sets up VAO, VBO and EBO buffers
		void SetupBuffers();

		//Returns true if ray intersects with any face
		bool Intersects(const glm::vec3 hitCoordinates);
		//Used for updating BoundingBox bounds when Mesh is transformed
		void UpdateBounds(const glm::mat4& transformationMatrix);
		//Draws BoundingBox around Mesh
		void Draw(Shader& shaderProgram, Camera& camera);
};