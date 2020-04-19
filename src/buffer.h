#pragma once
#include <glm/glm.hpp>

#define BUF_COORDS(x, y, w) ((y * w) + x)
#define EXT_BUF_COORDS(x, y, w, s, o)  ((y * w * s) + (x * s) + o) 

namespace rt {
    class PixelBuffer {
    public:
        PixelBuffer(int width, int height);
        PixelBuffer(char* path);
        void fill(glm::vec3 color);
        void savePNG(char* path);
        void saveBMP(char* path);
        void copyTo(PixelBuffer& buffer);
        int width;
        int height;
        glm::vec3* _buffer;
        ~PixelBuffer();
    };
}