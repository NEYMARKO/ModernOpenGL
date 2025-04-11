#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class PrintClass
{
	public:

		static void PrintVec3(const glm::vec3& vector);
		static void PrintQuat(const glm::quat& quaternion);
};