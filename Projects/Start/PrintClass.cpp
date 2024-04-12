#include "PrintClass.h"

void PrintClass::PrintVec3(const glm::vec3& vector)
{
	std::cout << "(x, y, z): (" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
}