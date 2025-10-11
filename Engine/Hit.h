#pragma once
#include <glm/glm.hpp>
struct SceneEntity;

struct Hit {
    SceneEntity* obj;
    glm::vec3 point;
};
