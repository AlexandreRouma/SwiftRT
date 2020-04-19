#include <material.h>

namespace rt::mat {
    material_t lambertian(glm::vec3 albedo) {
        material_t mat;
        mat.textureId = -1;
        mat.diffuseColor = albedo;
        mat.diffuseFactor = 1.0f;
        mat.reflectFactor = 0.0f;
        mat.traverseFactor = 0.0f;
        mat.emissiveFactor = 0.0f;
        return mat;
    }

    material_t lambertian(int textureId) {
        material_t mat;
        mat.textureId = textureId;
        mat.diffuseFactor = 1.0f;
        mat.reflectFactor = 0.0f;
        mat.traverseFactor = 0.0f;
        mat.emissiveFactor = 0.0f;
        return mat;
    }

    material_t light(glm::vec3 albedo) {
        material_t mat;
        mat.textureId = -1;
        mat.emissiveColor = albedo;
        mat.diffuseFactor = 0.0f;
        mat.reflectFactor = 0.0f;
        mat.traverseFactor = 0.0f;
        mat.emissiveFactor = 1.0f;
        return mat;
    }

    material_t glass(glm::vec3 albedo) {
        material_t mat;
        mat.textureId = -1;
        mat.traverseColor = albedo;
        mat.diffuseFactor = 0.0f;
        mat.reflectFactor = 0.0f;
        mat.traverseFactor = 1.0f;
        mat.emissiveFactor = 0.0f;
        mat.refractiveIndex = 1.517;
        return mat;
    }

    material_t mirror(glm::vec3 albedo) {
        material_t mat;
        mat.textureId = -1;
        mat.reflectColor = albedo;
        mat.diffuseColor = albedo;
        mat.diffuseFactor = 0.0f;
        mat.reflectFactor = 1.0f;
        mat.traverseFactor = 0.0f;
        mat.emissiveFactor = 0.0f;
        return mat;
    }
}