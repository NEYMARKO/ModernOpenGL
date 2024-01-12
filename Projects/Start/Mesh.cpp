#include "Mesh.h"
#include "Lighting.h"


Mesh::Mesh(MeshLoader* meshLoader, glm::vec3 objectPos, float id)
{
	this->meshLoader = meshLoader;
	this->objectPos = objectPos;
	this->id = id;
	setupMesh();
	this->boundingBox = new BoundingBox(meshLoader->minExtremes, meshLoader->maxExtremes, *this);
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();

	if (this->boundingBox != nullptr) {
		delete this->boundingBox;
		this->boundingBox = nullptr;  
	}
}

void Mesh::setupMesh()
{
	this->mVAO.Bind();
	this->mVBO = VBO(this->meshLoader->vertices);
	this->mEBO = EBO(this->meshLoader->indices);

	this->mVAO.LinkVBO(mVBO, 0, 3, sizeof(Vertex), 0);
	this->mVAO.LinkVBO(mVBO, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//this->mVAO.LinkVBO(mVBO, 2, 3, sizeof(Vertex), (void*)offsetof(Vertex, color));

	this->mVAO.Unbind();
	this->mVBO.Unbind();
	this->mEBO.Unbind();

}

void Mesh::ChangeColor(const glm::vec3& color)
{
	this->color = color;
}

void Mesh::Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting)
{
	shaderProgram.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->objectPos);
	model = glm::scale(model, glm::vec3(this->meshLoader->scalingFactor, this->meshLoader->scalingFactor, this->meshLoader->scalingFactor));

	shaderProgram.SetMat4("model", model);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("objectColor", this->color);
	shaderProgram.SetVec3("lightColor", lighting.lightColor);
	shaderProgram.SetVec3("lightPos", lighting.position);

	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();

	this->boundingBox->Draw(boundingBoxShaderProgram, camera);
}