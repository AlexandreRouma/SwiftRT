#include <buffer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace rt {
    PixelBuffer::PixelBuffer(int width, int height) {
        this->width = width;
        this->height = height;
        this->_buffer = new glm::vec3[width * height];
    }

    PixelBuffer::PixelBuffer(char* path) {
        int w;
        int h;
        int comp;
        unsigned char* image = stbi_load(path, &w, &h, &comp, STBI_rgb);
        
        this->width = w;
        this->height = h;
        this->_buffer = new glm::vec3[w * h];

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                float r = (float)image[EXT_BUF_COORDS(x, y, w, comp, 0)] / 255.0f;
                float g = (float)image[EXT_BUF_COORDS(x, y, w, comp, 1)] / 255.0f;
                float b = (float)image[EXT_BUF_COORDS(x, y, w, comp, 2)] / 255.0f;
                this->_buffer[(y * w) + x] = glm::vec3(r, g, b);
            }
        }

        free(image);
    }

    PixelBuffer::~PixelBuffer() {
        //this->width = 0;
        //this->height = 0;
        //delete[] this->_buffer;
    }

    void toRGBA(PixelBuffer buf, char* rgba) {
        for (int y = 0; y < buf.height; y++) {
            for (int x = 0; x < buf.width; x++) {
                rgba[(y * buf.width * 4) + (x * 4) + 0] = (buf._buffer[(y * buf.width) + x].r * 255);
                rgba[(y * buf.width * 4) + (x * 4) + 1] = (buf._buffer[(y * buf.width) + x].g * 255);
                rgba[(y * buf.width * 4) + (x * 4) + 2] = (buf._buffer[(y * buf.width) + x].b * 255);
                rgba[(y * buf.width * 4) + (x * 4) + 3] = 255;
            }
        }
    }

    void PixelBuffer::savePNG(char* path) {
        char* pixels = (char*)malloc(this->width * this->height * 4);
        toRGBA(*this, pixels);
        stbi_write_png(path, this->width, this->height, 4, pixels, this->width * 4);
        free(pixels);
    }

    void PixelBuffer::saveBMP(char* path) {
        char* pixels = (char*)malloc(this->width * this->height * 4);
        toRGBA(*this, pixels);
        stbi_write_bmp(path, this->width, this->height, 4, pixels);
        free(pixels);
    }

    void PixelBuffer::fill(glm::vec3 color) {
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < this->width; x++) {
                this->_buffer[(y * this->width) + x] = color;
            }
        }
    }

    void PixelBuffer::copyTo(PixelBuffer& buffer) {
        if (buffer.width != this->width || buffer.height != this->height) {
            return;
        }
        int count = buffer.width * buffer.height;
        for (int i = 0; i < count; i++) {
            buffer._buffer[i] = this->_buffer[i];
        }
    }
}