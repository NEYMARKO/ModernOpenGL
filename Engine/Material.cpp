#include "Shader.h"
#include "Material.h"

Material::Material(Shader* shaderProgram, glm::vec3 diffuse)
	: mShaderProgram{ shaderProgram }, mDiffuse{ diffuse }
{
	mAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	mSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	mShininess = 32.0f;
}

//void Material::setUniforms()
//{
//	mShaderProgram->SetVec3("material.ambient", mAmbient);
//	mShaderProgram->SetVec3("material.diffuse", mDiffuse);
//	mShaderProgram->SetVec3("material.specular", mSpecular);
//	mShaderProgram->SetFloat("material.shininess", mShininess);
//}