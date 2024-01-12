#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "MeshLoader.h"
#include "BoundingBox.h"

class Lighting;

class Mesh
{
	private:
		
		VBO mVBO;
		EBO mEBO;
		
		glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);

		//Binds buffers, fills VAO & EBO with data, in the end unbinds buffers
		void setupMesh();
	public:
		BoundingBox* boundingBox;
		MeshLoader* meshLoader;

		glm::vec3 objectPos;
		float scalingFactor;
		float id;
		VAO mVAO;
		
		Mesh(MeshLoader* meshLoader, glm::vec3 objectPos, float id);
		~Mesh();
		void ChangeColor(const glm::vec3& color);
		//Binds VAO, calculates MVP matrices, assigns uniforms and draws object using EBO info
		void Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting);
};