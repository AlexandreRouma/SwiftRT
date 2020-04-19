#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <stdio.h>

#define BUF_COORDS(x, y, w) ((y * w) + x) 

namespace rt {
    struct ray_t {
        glm::vec3 org;
        glm::vec3 dir;
    };

    class Camera {
    public:
        Camera(int width, int height, float fov);
        ray_t getRay(int x, int y, bool antialias);
        
    private:
        glm::vec3 _pos;
        glm::vec3 _rot;
        glm::vec3 _org;
        float _sDist;
        float _width;
        float _height;
        float _hOffset;
        float _hScale;
        float _AAWobble;
    };
}