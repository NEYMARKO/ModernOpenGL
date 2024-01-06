#include "Mesh.h"
#include "Lighting.h"


Mesh::Mesh(const char* filePath, glm::vec3 objectPos, float id)
{
	MeshLoader meshLoader(filePath, this);
	this->scalingFactor = meshLoader.scalingFactor;
	this->objectPos = objectPos;
	setupMesh();
	this->boundingBox = new BoundingBox(meshLoader.minExtremes, meshLoader.maxExtremes, *this);
	this->id = id;
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();
	//std::cout << "ID: " << id << std::endl;
	//std::cout << "BEFORE: " << this->boundingBox << std::endl;
	if (this->boundingBox != nullptr) {
		delete this->boundingBox;
		this->boundingBox = nullptr;  // Set to nullptr after deletion to avoid double deletion
	}
	//std::cout << "AFTER: " << this->boundingBox << std::endl;

	//std::cout << "DELETED ALL BUFFERS" << std::endl;
}

void Mesh::setupMesh()
{
	this->mVAO.Bind();
	this->mVBO = VBO(vertices);
	this->mEBO = EBO(indices);

	this->mVAO.LinkVBO(mVBO, 0, 3, sizeof(Vertex), 0);
	this->mVAO.LinkVBO(mVBO, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	this->mVAO.Unbind();
	this->mVBO.Unbind();
	this->mEBO.Unbind();

}

void Mesh::Draw(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting)
{
	shaderProgram.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->objectPos);
	model = glm::scale(model, glm::vec3(this->scalingFactor, this->scalingFactor, this->scalingFactor));

	shaderProgram.SetMat4("model", model);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	shaderProgram.SetVec3("lightColor", lighting.lightColor);
	shaderProgram.SetVec3("lightPos", lighting.position);

	
	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();

	boundingBoxShaderProgram.Activate();
	this->boundingBox->Draw(boundingBoxShaderProgram, camera);
}
