#pragma once
#include <glm/glm.hpp>

namespace rt::mat {
    struct material_t {
        glm::vec3 diffuseColor;
        glm::vec3 reflectColor;
        glm::vec3 traverseColor;
        glm::vec3 emissiveColor;
        float diffuseFactor;
        float reflectFactor;
        float traverseFactor;
        float emissiveFactor;
        int textureId;
        float refractiveIndex;
    };

    material_t lambertian(glm::vec3 albedo);
    material_t lambertian(int textureId);
    material_t light(glm::vec3 albedo);
    material_t glass(glm::vec3 albedo);
    material_t mirror(glm::vec3 albedo);
}