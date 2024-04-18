#include "PrintClass.h"

void PrintClass::PrintVec3(const glm::vec3& vector)
{
	std::cout << "(x, y, z): (" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
}

void PrintClass::PrintQuat(const glm::quat& quaternion)
{
    std::cout << "(w, x, y, z): ("
        << quaternion.w << ", "
        << quaternion.x << ", "
        << quaternion.y << ", "
        << quaternion.z << ")" << std::endl;
}