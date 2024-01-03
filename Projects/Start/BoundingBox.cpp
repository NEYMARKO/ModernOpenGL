#include "Mesh.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh) : parentMesh(parentMesh)
{
	//Bounds: [xmin, xmax, ymin, ymax, zmin, zmax]
	for (int i = 0; i < 3; i++)
	{
		this->bounds.push_back(minExtremes[i]);
		this->bounds.push_back(maxExtremes[i]);
	}
	Initialize();
	SetupBuffers();
}
BoundingBox::~BoundingBox()
{
	boxVAO.Delete();
	boxVBO.Delete();
	boxEBO.Delete();
	//std::cout << "Deleted bounding box" << std::endl;
}
void BoundingBox::Initialize()
{
	//std::cout << "Creating bounding box" << std::endl;
	float xmin = this->bounds[0];
	float xmax = this->bounds[1];
	float ymin = this->bounds[2];
	float ymax = this->bounds[3];
	float zmin = this->bounds[4];
	float zmax = this->bounds[5];

	this->vertices = {
		glm::vec3(xmin, ymin, zmin), // 0
		glm::vec3(xmax, ymin, zmin), // 1
		glm::vec3(xmax, ymax, zmin), // 2
		glm::vec3(xmin, ymax, zmin), // 3
		glm::vec3(xmin, ymin, zmax), // 4
		glm::vec3(xmax, ymin, zmax), // 5
		glm::vec3(xmax, ymax, zmax), // 6
		glm::vec3(xmin, ymax, zmax)  // 7
	};

	this->indices = {
		// Bottom face
		0, 1, 1, 2, 2, 3, 3, 0,
		// Top face
		4, 5, 5, 6, 6, 7, 7, 4,
		// Side edges
		0, 4, 1, 5, 2, 6, 3, 7
	};
}

void BoundingBox::SetupBuffers()
{
	this->boxVAO.Bind();
	this->boxVBO = VBO(this->vertices);
	this->boxEBO = EBO(this->indices);

	this->boxVAO.LinkVBO(boxVBO, 0, sizeof(glm::vec3), 0);

	this->boxVAO.Unbind();
	this->boxVBO.Unbind();
	this->boxEBO.Unbind();
}

void BoundingBox::Draw(Shader& shaderProgram, Camera& camera)
{
	shaderProgram.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->parentMesh.objectPos);
	model = glm::scale(model, glm::vec3(this->parentMesh.scalingFactor, this->parentMesh.scalingFactor, this->parentMesh.scalingFactor));

	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	camera.ViewProjectionMatrix(camera.lookAtPosition, shaderProgram);

	boxVAO.Bind();

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

	boxVAO.Unbind();
}