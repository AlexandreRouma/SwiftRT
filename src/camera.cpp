#include <camera.h>

namespace rt {
    Camera::Camera(int width, int height, float fov) {
        this->_sDist = 0.5f / glm::tan(glm::radians(fov / 2));
        this->_org = glm::vec3(0.0f, 0.0f, -this->_sDist);
        this->_width = width;
        this->_height = height;
        this->_hScale = -(this->_height / this->_width);
        this->_hOffset = -(this->_hScale / 2);
        this->_AAWobble = 1.0f / (2.0f * this->_width);
        printf("%f\n", this->_sDist);
    }

    ray_t Camera::getRay(int x, int y, bool antialias) {
        ray_t ray;
        ray.org = this->_org;
        ray.dir = glm::normalize(glm::vec3((x / this->_width) - 0.5f,  ((y * this->_hScale) / this->_height) + this->_hOffset, this->_sDist));
        if (antialias) {
            ray.dir.x += glm::linearRand(-this->_AAWobble, this->_AAWobble);
            ray.dir.y += glm::linearRand(-this->_AAWobble, this->_AAWobble);
        }
        return ray;
    }
};