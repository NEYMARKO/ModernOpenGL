#include "Mesh.h"
#include "BoundingBox.h"
BoundingBox::BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh) : parentMesh(parentMesh)
{
	this->minExtremes = minExtremes;
	this->maxExtremes = maxExtremes;
	Initialize();
	SetupBuffers();
	VerticesToWorld();
}
BoundingBox::~BoundingBox()
{
	boxVAO.Delete();
	boxVBO.Delete();
	boxEBO.Delete();
	std::cout << "Deleted bounding box of mesh with id: " << this->parentMesh.id << std::endl;
}
void BoundingBox::Initialize()
{
	float xmin = this->minExtremes[0];
	float ymin = this->minExtremes[1];
	float zmin = this->minExtremes[2];
	float xmax = this->maxExtremes[0];
	float ymax = this->maxExtremes[1];
	float zmax = this->maxExtremes[2];
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

	this->boxVAO.LinkVBO(boxVBO, 0, 3, sizeof(glm::vec3), 0);

	this->boxVAO.Unbind();
	this->boxVBO.Unbind();
	this->boxEBO.Unbind();
}


void BoundingBox::VerticesToWorld()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->parentMesh.objectPos);
	model = glm::scale(model, glm::vec3(this->parentMesh.scalingFactor, this->parentMesh.scalingFactor, this->parentMesh.scalingFactor));

	this->minExtremes = glm::vec3(model * glm::vec4(this->minExtremes, 1.0));
	this->maxExtremes = glm::vec3(model * glm::vec4(this->maxExtremes, 1.0));

}

bool BoundingBox::Intersects(Camera& camera, float step)
{
	Ray* ray = camera.ray;

	glm::vec3 rayPoint = ray->GetRayStart() + ray->GetRayDirection() * step;
	for (int i = 0; i < 3; i++)
	{
		if (rayPoint[i] > maxExtremes[i] || rayPoint[i] < minExtremes[i]) return false;
	}
	std::cout << "Clicked on: " << this->parentMesh.name << std::endl;
	return true;
}

void BoundingBox::Draw(Shader& shaderProgram, Camera& camera)
{
	shaderProgram.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->parentMesh.objectPos);
	model = glm::scale(model, glm::vec3(this->parentMesh.scalingFactor, this->parentMesh.scalingFactor, this->parentMesh.scalingFactor));

	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	camera.ViewProjectionMatrix(shaderProgram);

	boxVAO.Bind();

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

	boxVAO.Unbind();
}