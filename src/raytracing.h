#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include <buffer.h>
#include <material.h>
#include <camera.h>
#include <float.h>

namespace rt {
    struct triangle_t {
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;
        glm::vec3 norm;
        mat::material_t mat;
        glm::vec2 u;
        glm::vec2 v;
        glm::vec2 o;
    };

    void trace(Camera cam, std::vector<triangle_t>& tris, std::vector<PixelBuffer>& textures, PixelBuffer& buffer);
    void depthMap(Camera cam, std::vector<triangle_t>& tris, PixelBuffer& buffer);
    void albedo(Camera cam, std::vector<triangle_t>& tris, PixelBuffer& buffer);

    void probe(Camera cam, std::vector<triangle_t>& tris, std::vector<PixelBuffer>& textures, PixelBuffer& albedo, PixelBuffer& normals);

    triangle_t createTri(glm::vec3 a, glm::vec3 b, glm::vec3 c, mat::material_t mat, glm::vec2 u, glm::vec2 v, glm::vec2 o);
}