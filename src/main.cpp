#include <stdio.h>
#include <glm/glm.hpp>
#include <raytracing.h>
#include <postproc.h>
#include <chrono>
#include <algorithm>
#include <model.h>
#include <cmath>
#include <limits>

using namespace std::chrono;

void addFace(std::vector<rt::triangle_t>& tris, glm::vec3 a,  glm::vec3 b, glm::vec3 c, glm::vec3 d, rt::mat::material_t mat) {
    tris.push_back(rt::createTri(a, b, c, mat, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(a, d, c, mat, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
}

void addCube(std::vector<rt::triangle_t>& tris, glm::vec3 center, glm::vec3 rotation, float width, float height, float depth, rt::mat::material_t mat) {
    // TODO: Add rotation
    glm::vec3 ba = glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f) + center;
    glm::vec3 bb = glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f) + center;
    glm::vec3 bc = glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f) + center;
    glm::vec3 bd = glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f) + center;
    glm::vec3 ta = glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f) + center;
    glm::vec3 tb = glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f) + center;
    glm::vec3 tc = glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f) + center;
    glm::vec3 td = glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f) + center;
    addFace(tris, ba, bb, bc, bd, mat); // Bottom
    addFace(tris, ta, tb, tc, td, mat); // Top
    addFace(tris, td, ta, ba, bd, mat); // Left
    addFace(tris, tc, tb, bb, bc, mat); // Right
    addFace(tris, ta, tb, bb, ba, mat); // Back
    addFace(tris, td, tc, bc, bd, mat); // Front
}

int main() {
    printf("Starting!\n");
    std::vector<rt::PixelBuffer> textures;
    rt::PixelBuffer buffer(800, 800);

    rt::PixelBuffer albedoBuffer(buffer.width, buffer.height);
    rt::PixelBuffer normalBuffer(buffer.width, buffer.height);

    // Load textures
    textures.push_back(rt::PixelBuffer("C:/tmp/test.png"));
    textures.push_back(rt::PixelBuffer("C:/tmp/tiles.png"));

    rt::mat::material_t redWall = rt::mat::lambertian(glm::vec3(1.0f, 0.3f, 0.1f));
    rt::mat::material_t greenWall = rt::mat::lambertian(glm::vec3(0.3f, 1.0f, 0.3f));
    rt::mat::material_t whiteWall = rt::mat::lambertian(glm::vec3(1.0f, 1.0f, 1.0f));
    rt::mat::material_t whiteLight = rt::mat::light(glm::vec3(1.0f, 1.0f, 1.0f) * 30.0f);
    rt::mat::material_t cube1mat = rt::mat::lambertian(0);
    rt::mat::material_t cube2mat = rt::mat::lambertian(0);
    rt::mat::material_t tiles = rt::mat::lambertian(1);
    rt::mat::material_t glass = rt::mat::glass(glm::vec3(0.0f, 0.0f, 1.0f));
    rt::mat::material_t mirror = rt::mat::glass(glm::vec3(1.0f, 1.0f, 1.0f));

    tiles.diffuseFactor = 0.8f;
    tiles.reflectFactor = 0.2f;
    tiles.reflectColor = glm::vec3(1.0f, 1.0f, 1.0f);

    //glass.refractiveIndex = 1.0f;
    
    std::vector<rt::triangle_t> tris;

    printf("Loading model\n");
    //rt::loadOBJ(tris, "C:/tmp/OpenGL_TEST/models/rabbit.obj", glass, glm::vec3(-2.0f, -1.0f, 14));
    printf("Model loaded!\n");

    tris.push_back(rt::createTri(glm::vec3(-5, -5, 8), glm::vec3(5, -5, 8), glm::vec3(5, -5, 18), tiles, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, -5, 8), glm::vec3(-5, -5, 18), glm::vec3(5, -5, 18), tiles, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, 5, 8), glm::vec3(5, 5, 8), glm::vec3(5, 5, 18), whiteWall, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, 5, 8), glm::vec3(-5, 5, 18), glm::vec3(5, 5, 18), whiteWall, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, -5, 18), glm::vec3(5, -5, 18), glm::vec3(5, 5, 18), whiteWall, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, -5, 18), glm::vec3(-5, 5, 18), glm::vec3(5, 5, 18), whiteWall, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, -5, 8), glm::vec3(-5, -5, 18), glm::vec3(-5, 5, 18), redWall, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(-5, -5, 8), glm::vec3(-5, 5, 8), glm::vec3(-5, 5, 18), redWall, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    tris.push_back(rt::createTri(glm::vec3(5, -5, 8), glm::vec3(5, -5, 18), glm::vec3(5, 5, 18), greenWall, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(5, -5, 8), glm::vec3(5, 5, 8), glm::vec3(5, 5, 18), greenWall, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    tris.push_back(rt::createTri(glm::vec3(-1, 4.98f, 12), glm::vec3(1, 4.98f, 12), glm::vec3(1, 4.98f, 14), whiteLight, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
    tris.push_back(rt::createTri(glm::vec3(-1, 4.98f, 12), glm::vec3(-1, 4.98f, 14), glm::vec3(1, 4.98f, 14), whiteLight, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    addFace(tris, glm::vec3(-20.0f, 20.0f, -2.0f), glm::vec3(20.0f, 20.0f, -2.0f),  glm::vec3(20.0f, -20.0f, -2.0f),  glm::vec3(-20.0f, -20.0f, -2.0f), whiteWall);

    addCube(tris, glm::vec3(-3.0f, -3.5f, 14), glm::vec3(), 2.5f, 2.5f, 2.5f, glass);
    addCube(tris, glm::vec3(3.0f, -3.5f, 14), glm::vec3(), 2.5f, 2.5f, 2.5f, cube2mat);

    rt::Camera cam(buffer.width, buffer.height, 63.4f);

    int sampleCount = 100;

    auto start = high_resolution_clock::now();
    printf("Sampling albedos and normals for intel denoise\n");
    rt::probe(cam, tris, textures, albedoBuffer, normalBuffer);
    albedoBuffer.savePNG("../output/albedos.png");
    normalBuffer.savePNG("../output/normals.png");

    //return 0; // Used when testing big models

    for (int i = 0; i < sampleCount; i++) {
        printf("Rendering sample %d\n", i);
        rt::trace(cam, tris, textures, buffer);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds = (float)duration.count() / 1000000.0f;
    printf("Render time: %fs, FPS: %f\n", seconds, (float)sampleCount / seconds);

    printf("Processing...\n");

    rt::postproc::normalize(buffer, (float)sampleCount);
    buffer.savePNG("../output/raw_output.png");

    rt::postproc::intelDenoise(buffer, albedoBuffer, normalBuffer);
    rt::postproc::gamma(buffer, 2.2f);

    buffer.savePNG("../output/final_output.png");

    printf("Done\n");
    return 0;
}