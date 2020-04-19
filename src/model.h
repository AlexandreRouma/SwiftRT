#pragma once
#include <glm/glm.hpp>
#include <raytracing.h>
#include <string>
#include <fstream>
#include <material.h>

namespace rt {
    void loadOBJ(std::vector<triangle_t>& tris, char* path, mat::material_t mat, glm::vec3 position);
}