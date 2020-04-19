#pragma once
#include <glm/glm.hpp>
#include <buffer.h>

namespace rt::postproc {
    void normalize(PixelBuffer& buffer, float max);
    void gamma(PixelBuffer& buffer, float gamma);
    void intelDenoise(PixelBuffer& buffer);
    void intelDenoise(PixelBuffer& buffer, PixelBuffer& albedo);
    void intelDenoise(PixelBuffer& buffer, PixelBuffer& albedo, PixelBuffer& normals);
}