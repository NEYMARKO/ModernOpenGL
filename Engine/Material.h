#pragma once
#include <glm/glm.hpp>

class Shader;

class Material
{
private:
	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
	float mShininess;

	Shader* mShaderProgram;

public:
	Material(Shader* shaderProgram, glm::vec3 diffuse);

	Shader* getShaderProgram() { return mShaderProgram; }
	//void setUniforms();

	glm::vec3 getDiffuse() { return mDiffuse; }
};