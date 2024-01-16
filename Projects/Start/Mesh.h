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

		glm::mat4 translationMatrix = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::mat4 scalingMatrix = glm::mat4(1.0f);
		glm::mat4 finalMatrix = glm::mat4(1.0f);
		//Binds buffers, fills VAO & EBO with data, in the end unbinds buffers
		void setupMesh();
	public:
		BoundingBox* boundingBox;
		MeshLoader* meshLoader;

		glm::vec3 objectPos = glm::vec3(0.0, 0.0, 0.0);
		float scalingFactor;
		float id;
		VAO mVAO;
		
		Mesh(MeshLoader* meshLoader, glm::vec3 objectPos, float id);
		~Mesh();
		void ChangeColor(const glm::vec3& color);
		void InitialTransform(glm::vec3 translation, float scale);
		void Translate(const glm::vec3& newPosition);
		void Rotate(const glm::vec3& rotationVector, float angle);
		void Scale(float scale);
		void CalculateFinalMatrix();
		glm::mat4 GetFinalMatrix();
		//Binds VAO, calculates MVP matrices, assigns uniforms and draws object using EBO info
		void Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting);
};