#include <raytracing.h>

#define M_PI            3.14159265358979323846
#define M_2_PI          2 * M_PI

namespace rt {

    float cRand() {
        return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }

    float cRand2() {
        return glm::linearRand(0.0f, 1.0f);
    }

    void printVec3(glm::vec3 vec) {
        printf("(%f; %f; %f)\n", vec.x, vec.y, vec.z);
    }

    glm::vec3 CosineBRDF(glm::vec3 normal) {
        float r0 = cRand2();
        float r1 = cRand2();

        glm::vec3 uu = glm::normalize(glm::cross(normal, { 0.0f, 1.0f, 1.0f }));
        glm::vec3 vv = glm::cross(uu, normal);

        float ra = sqrtf(r1);

        float rx = ra * cosf(M_2_PI * r0);
        float ry = ra * sinf(M_2_PI * r0);

        float rz = sqrtf(1.0f - r1);

        return glm::normalize(rx * uu + ry * vv + rz * normal);
    }

    glm::vec3 castRay(std::vector<triangle_t>& tris, int triCount, std::vector<PixelBuffer>& textures, ray_t ray, int maxBounce, int& bounceCount, int lastTri, float cRI) {
        if (bounceCount >= maxBounce) {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }
        //int triCount = tris.size();
        int triIdx = -1;
        float dist = 0.0f;
        float sDist = INFINITY;
        glm::vec2 bari;
        glm::vec2 sBari;
        for (int i = 0; i < triCount; i++) {
            if (i == lastTri) {
                continue;
            }
            if (!glm::intersectRayTriangle(ray.org, ray.dir, tris[i].b, tris[i].a, tris[i].c, bari, dist)) {
                continue;
            }
            if (dist > 0.0f && dist < sDist) {
                sDist = dist;
                triIdx = i;
                sBari = bari;
            }
        }
        if (triIdx < 0) {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }
        
        triangle_t tri = tris[triIdx];
        glm::vec3 normal = glm::dot(tri.norm, ray.dir) > 0 ? -tri.norm : tri.norm;

        bounceCount++;

        float rSelect = glm::linearRand(0.0f, 1.0f);
        if (rSelect <= tri.mat.diffuseFactor) {
            ray_t reflected;
            reflected.dir = CosineBRDF(normal);
            reflected.org = ray.org + (ray.dir * sDist);
            if (tri.mat.textureId != -1) {
                PixelBuffer texture = textures[tri.mat.textureId];
                glm::vec2 texCoord = (sBari.x * tri.u) + (sBari.y * tri.v) + tri.o;
                return castRay(tris, triCount, textures, reflected, maxBounce, bounceCount, triIdx, cRI) *
                        texture._buffer[((int)(texCoord.y * (texture.height - 1)) * texture.width) + (int)(texCoord.x * (texture.width - 1))];
            }
            else {
                return castRay(tris, triCount, textures, reflected, maxBounce, bounceCount, triIdx, cRI) * tri.mat.diffuseColor;
            }
        }
        else if (rSelect <= tri.mat.diffuseFactor + tri.mat.reflectFactor) {
            ray_t reflected;
            reflected.dir = glm::reflect(ray.dir, normal);
            reflected.org = ray.org + (ray.dir * sDist);
            return castRay(tris, triCount, textures, reflected, maxBounce, bounceCount, triIdx, cRI) * tri.mat.reflectColor;
        }
        else if (rSelect <= tri.mat.diffuseFactor + tri.mat.reflectFactor + tri.mat.traverseFactor) {
            float nRI = (cRI == 1.0f) ? tri.mat.refractiveIndex : 1.0f;
            float critical = (nRI < cRI) ? asinf(nRI / cRI) : 100;
            ray_t reflected;

            //printf("cRI: %f, nRI: %f, critical: %f, actual: %f, \n", cRI, nRI, critical, glm::angle(-ray.dir, normal));

            reflected.org = ray.org + (ray.dir * sDist);

            if (glm::angle(-ray.dir, normal) < critical) {
                reflected.dir = glm::refract(ray.dir, normal, cRI / nRI);
                //reflected.org += (normal * -0.00001f);
            }
            else {
                nRI = cRI;
                reflected.dir = glm::reflect(ray.dir, normal); // Total internal reflection
                //reflected.org += (normal * 0.00001f);
            }

            return castRay(tris, triCount, textures, reflected, maxBounce, bounceCount, triIdx, nRI) * tri.mat.traverseColor;
        }
        else if (rSelect <= tri.mat.diffuseFactor + tri.mat.reflectFactor + tri.mat.traverseFactor + tri.mat.emissiveFactor) {
            return tri.mat.emissiveColor;
        }
    }

    void trace(Camera cam, std::vector<triangle_t>& tris, std::vector<PixelBuffer>& textures, PixelBuffer& buffer) {
        int triCount = tris.size();
        #pragma omp parallel for schedule(dynamic)
        for (int y = 0; y < buffer.height; y++) {
            for (int x = 0; x < buffer.width; x++) {
                ray_t camRay = cam.getRay(x, y, true);
                int bounces = 0;
                buffer._buffer[BUF_COORDS(x, y, buffer.width)] += castRay(tris, triCount, textures, camRay, 7, bounces, -1, 1.0f);
            }
        }
    }

    void depthMap(Camera cam, std::vector<triangle_t>& tris, PixelBuffer& buffer) {
        int triCount = tris.size();
        int triIdx = -1;
        float dist = 0.0f;
        float sDist = INFINITY;
        glm::vec2 bari;
        for (int y = 0; y < buffer.height; y++) {
            for (int x = 0; x < buffer.width; x++) {
                ray_t camRay = cam.getRay(x, y, false);
                triIdx = -1;
                sDist = INFINITY;
                for (int i = 0; i < triCount; i++) {
                    if (!glm::intersectRayTriangle(camRay.org, camRay.dir, tris[i].a, tris[i].b, tris[i].c, bari, dist)) {
                        continue;
                    }
                    if (dist >= 0 && dist < sDist) {
                        sDist = dist;
                        triIdx = i;
                    }
                }
                if (triIdx < 0) {
                    continue;
                }
                buffer._buffer[BUF_COORDS(x, y, buffer.width)] = glm::vec3(sDist / 21.0f, sDist / 21.0f, sDist / 21.0f);
            }
        }
    }
    
    void albedo(Camera cam, std::vector<triangle_t>& tris, PixelBuffer& buffer) {
        int triCount = tris.size();
        int triIdx = -1;
        float dist = 0.0f;
        float sDist = INFINITY;
        glm::vec2 bari;
        for (int y = 0; y < buffer.height; y++) {
            for (int x = 0; x < buffer.width; x++) {
                ray_t camRay = cam.getRay(x, y, false);
                triIdx = -1;
                sDist = INFINITY;
                for (int i = 0; i < triCount; i++) {
                    if (!glm::intersectRayTriangle(camRay.org, camRay.dir, tris[i].a, tris[i].b, tris[i].c, bari, dist)) {
                        continue;
                    }
                    if (dist >= 0 && dist < sDist) {
                        sDist = dist;
                        triIdx = i;
                    }
                }
                if (triIdx < 0) {
                    continue;
                }
                buffer._buffer[BUF_COORDS(x, y, buffer.width)] = (tris[triIdx].mat.diffuseColor * tris[triIdx].mat.diffuseFactor) + 
                                                                (tris[triIdx].mat.reflectColor * tris[triIdx].mat.reflectFactor) +
                                                                (tris[triIdx].mat.traverseColor * tris[triIdx].mat.traverseFactor) +
                                                                (tris[triIdx].mat.emissiveColor * tris[triIdx].mat.emissiveFactor);
            }
        }
    }

    void probe(Camera cam, std::vector<triangle_t>& tris, std::vector<PixelBuffer>& textures, PixelBuffer& albedo, PixelBuffer& normals) {
        int triCount = tris.size();
        int triIdx = -1;
        float dist = 0.0f;
        float sDist = INFINITY;
        glm::vec2 bari;
        glm::vec2 sBari;
        for (int y = 0; y < albedo.height; y++) {
            for (int x = 0; x < albedo.width; x++) {
                ray_t camRay = cam.getRay(x, y, false);
                triIdx = -1;
                sDist = INFINITY;
                for (int i = 0; i < triCount; i++) {
                    if (!glm::intersectRayTriangle(camRay.org, camRay.dir, tris[i].b, tris[i].a, tris[i].c, bari, dist)) {
                        continue;
                    }
                    if (dist >= 0 && dist < sDist) {
                        sDist = dist;
                        triIdx = i;
                        sBari = bari;
                    }
                }
                if (triIdx < 0) {
                    continue;
                }
                glm::vec3 normal = glm::dot(tris[triIdx].norm, camRay.dir) > 0 ? -tris[triIdx].norm : tris[triIdx].norm;

                if (tris[triIdx].mat.textureId != -1) {
                    PixelBuffer texture = textures[tris[triIdx].mat.textureId];
                    glm::vec2 texCoord = (sBari.x * tris[triIdx].u) + (sBari.y * tris[triIdx].v) + tris[triIdx].o;
                    albedo._buffer[BUF_COORDS(x, y, albedo.width)] = texture._buffer[((int)(texCoord.y * (texture.height - 1)) * texture.width) + (int)(texCoord.x * (texture.width - 1))];
                }
                else {
                    albedo._buffer[BUF_COORDS(x, y, albedo.width)] = (tris[triIdx].mat.diffuseColor * tris[triIdx].mat.diffuseFactor) + 
                                                                (tris[triIdx].mat.reflectColor * tris[triIdx].mat.reflectFactor) +
                                                                (tris[triIdx].mat.traverseColor * tris[triIdx].mat.traverseFactor) +
                                                                (tris[triIdx].mat.emissiveColor * tris[triIdx].mat.emissiveFactor);
                }

                normals._buffer[BUF_COORDS(x, y, albedo.width)] = normal;
            }
        }
    }

    triangle_t createTri(glm::vec3 a, glm::vec3 b, glm::vec3 c, mat::material_t mat, glm::vec2 u, glm::vec2 v, glm::vec2 o) {
        triangle_t tri;
        tri.a = a;
        tri.b = b;
        tri.c = c;
        tri.u = u;
        tri.v = v;
        tri.o = o;
        tri.norm = glm::normalize(glm::triangleNormal(a, b, c));
        tri.mat = mat;
        return tri;
    }

    mat::material_t createDiffuseMat(glm::vec3 color) {
        mat::material_t mat;
        mat.diffuseColor = color;
        mat.diffuseFactor = 1.0f;
        mat.reflectFactor = 0.0f;
        mat.traverseFactor = 0.0f;
        return mat;
    }
}
