#include <postproc.h>
#include <OpenImageDenoise/oidn.hpp>

namespace rt::postproc {
    void normalize(PixelBuffer& buffer, float max) {
        int count = buffer.width * buffer.height;
        for (int i = 0; i < count; i++) {
            buffer._buffer[i] = glm::clamp(buffer._buffer[i], 0.0f, max) / max;
        }
    }

    void gamma(PixelBuffer& buffer, float gamma) {
        int count = buffer.width * buffer.height;
        for (int i = 0; i < count; i++) {
            buffer._buffer[i] = glm::pow(buffer._buffer[i], glm::vec3(1.0f / gamma, 1.0f / gamma, 1.0f / gamma));
        }
    }

    void intelDenoise(PixelBuffer& buffer) {
        PixelBuffer temp(buffer.width, buffer.height);
        oidn::DeviceRef device = oidn::newDevice();
        device.commit();
        oidn::FilterRef filter = device.newFilter("RT"); // generic ray tracing filter
        filter.setImage("color",  buffer._buffer,  oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("output", temp._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.set("hdr", false); // image is HDR
        filter.commit();
        filter.execute();
        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None) {
            printf("Error: %s\n", errorMessage);
        }
        temp.copyTo(buffer);
    }

    void intelDenoise(PixelBuffer& buffer, PixelBuffer& albedo) {
        PixelBuffer temp(buffer.width, buffer.height);
        oidn::DeviceRef device = oidn::newDevice();
        device.commit();
        oidn::FilterRef filter = device.newFilter("RT");
        filter.setImage("color",  buffer._buffer,  oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("albedo", albedo._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("output", temp._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.set("hdr", false);
        filter.commit();
        filter.execute();
        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None) {
            printf("Error: %s\n", errorMessage);
        }
        
        temp.copyTo(buffer);
    }

    void intelDenoise(PixelBuffer& buffer, PixelBuffer& albedo, PixelBuffer& normals) {
        PixelBuffer temp(buffer.width, buffer.height);
        oidn::DeviceRef device = oidn::newDevice();
        device.commit();
        oidn::FilterRef filter = device.newFilter("RT");
        filter.setImage("color",  buffer._buffer,  oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("albedo", albedo._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("normal", normals._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.setImage("output", temp._buffer, oidn::Format::Float3, buffer.width, buffer.height);
        filter.set("hdr", false);
        filter.commit();
        filter.execute();
        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None) {
            printf("Error: %s\n", errorMessage);
        }
        temp.copyTo(buffer);
    }
}