#include "Grid.h"

Grid::Grid(int size)
{
	this->size = size;
	SetupGrid();
}

void Grid::SetupGrid()
{
	for (int i = 0; i <= this->size; i++)
	{
		for (int j = 0; j <= this->size; j++)
		{
			this->vertices.push_back(glm::vec3(j, 0, i));
		}
	}

	for (int i = 0; i < this->size; i++)
	{
		for (int j = 0; j < this->size; j++)
		{
			//horizontal indices
			this->indices.push_back(i * this->size + j + i);
			this->indices.push_back(i * this->size + j + i + 1);
			//vertical indices
			this->indices.push_back(i * this->size + j + i);
			this->indices.push_back((i + 1) * this->size + j + i + 1);
		}
	}

	this->modelMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(-this->size / 2, 0.0f, -this->size / 2));

	this->gridVAO.Bind();
	this->gridVBO = VBO(this->vertices);
	this->gridEBO = EBO(this->indices);

	this->gridVAO.LinkVBO(gridVBO, 0, 3, sizeof(glm::vec3), 0);

	this->gridVAO.Unbind();
	this->gridVBO.Unbind();
	this->gridEBO.Unbind();
}
void Grid::Draw(Shader& shaderProgram, Camera& camera)
{
	//std::cout << "Drawing grid" << std::endl;
	shaderProgram.Activate();

	glLineWidth(2.5f);

	shaderProgram.SetMat4("model", this->modelMatrix);
	shaderProgram.SetVec3("cameraPos", camera.cameraPos);
	shaderProgram.SetVec3("lineColor", glm::vec3(0.329412f, 0.329412f, 0.329412f));

	camera.ViewProjectionMatrix(shaderProgram);

	gridVAO.Bind();

	glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, 0);

	gridVAO.Unbind();
}

Grid::~Grid()
{
	std::cout << "Grid deleted" << std::endl;
}