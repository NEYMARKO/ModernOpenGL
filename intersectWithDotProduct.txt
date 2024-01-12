#include "Mesh.h"
#include "BoundingBox.h"
BoundingBox::BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh) : parentMesh(parentMesh)
{
	//Bounds: [xmin, xmax, ymin, ymax, zmin, zmax]
	/*for (int i = 0; i < 3; i++)
	{
		this->bounds.push_back(minExtremes[i]);
		this->bounds.push_back(maxExtremes[i]);
	}*/
	this->minExtremes = minExtremes;
	this->maxExtremes = maxExtremes;
	Initialize();
	SetupBuffers();
	VerticesToWorld();
	UpdatePlanes();
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
	//std::cout << "Creating bounding box" << std::endl;
	/*float xmin = this->bounds[0];
	float xmax = this->bounds[1];
	float ymin = this->bounds[2];
	float ymax = this->bounds[3];
	float zmin = this->bounds[4];
	float zmax = this->bounds[5];*/

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

	this->trisIndices = {
		3, 2, 0,
		2, 1, 0,
		2, 6, 1,
		6, 5, 1,
		6, 7, 5,
		7, 4, 5,
		7, 3, 4,
		3, 0, 4,
		3, 6, 2,
		3, 7, 6,
		0, 1, 5,
		0, 5, 4
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
	/*
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "Name: " << this->parentMesh.name << std::endl;
	std::cout << "Object pos: (" << this->parentMesh.objectPos.x << ", " << this->parentMesh.objectPos.y << ", " << this->parentMesh.objectPos.z << ")" << std::endl;
	std::cout << "Scaling factor: " << this->parentMesh.scalingFactor << std::endl;
	
	std::cout << "BEFORE MODEL: " << std::endl;
	for (int i = 0; i < this->vertices.size(); i++)
	{
		std::cout << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
	}
	std::cout << "AFTER MODEL: " << std::endl;*/
	/*for (int i = 0; i < this->vertices.size(); i++)
	{
		this->vertices[i] = glm::vec3(model * glm::vec4(this->vertices[i], 1.0));
		std::cout << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
	}*/

	this->minExtremes = glm::vec3(model * glm::vec4(this->minExtremes, 1.0));
	this->maxExtremes = glm::vec3(model * glm::vec4(this->maxExtremes, 1.0));

}

Plane BoundingBox::CalculatePlaneCoefficients(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	/*float A = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
	float B = -(p2.x - p1.x) * (p3.z - p1.z) + (p2.z - p1.z) * (p3.x - p1.x);
	float C = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);*/
	glm::vec3 orthoVector = glm::cross(p1 - p2, p3 - p2);
	float A = orthoVector.x;
	float B = orthoVector.y;
	float C = orthoVector.z;
	float D = -p1.x * A - p1.y * B - p1.z * C;

	Plane plane = { glm::vec3(A, B, C), D};
	return plane;
}

void BoundingBox::UpdatePlanes()
{
	if (this->planeCoefficients.size() == 0)
	{
		for (int i = 0; i < trisIndices.size() - 2; i += 3)
		{
			this->planeCoefficients.push_back(CalculatePlaneCoefficients(
				this->vertices[this->trisIndices[i]],
				this->vertices[this->trisIndices[i + 1]],
				this->vertices[this->trisIndices[i + 2]]));
		}
	}
	else 
	{
		for (int i = 0; i < trisIndices.size() - 2; i += 3)
		{
			this->planeCoefficients[i / 3] = CalculatePlaneCoefficients(
				this->vertices[this->trisIndices[i]],
				this->vertices[this->trisIndices[i + 1]],
				this->vertices[this->trisIndices[i + 2]]);
		}
	}
}

bool BoundingBox::Intersects(Camera& camera, float step)
{
	Ray* ray = camera.ray;
	//glm::vec3 rayStart = ray->GetRayStart();
	//glm::vec3 rayDirection = ray->GetRayDirection();

	//glm::vec3 rayPoint = rayStart + rayDirection * step;
	//float result = 0.0f;
	////std::cout << "RAY START INT: (" << ray.GetRayStart().x << ", " << ray.GetRayStart().y << ", " << ray.GetRayStart().z << ")" << std::endl;
	//for (int i = 0; i < this->planeCoefficients.size(); i++)
	//{
	//	//std::cout << "RAY POINT: (" << rayPoint.x << ", " << rayPoint.y << ", " << rayPoint.z << ")" << std::endl;
	//	result = glm::dot(rayPoint, this->planeCoefficients[i].normal) + this->planeCoefficients[i].D;
	//	//std::cout << "RESULT: " << result << std::endl;
	//	if (result > 0)
	//	{
	//		return false;
	//	}
	//}

	//return true;

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