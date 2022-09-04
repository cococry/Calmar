#pragma once

#include "calmar/core/defines.hpp"

#include "rendering_backend.hpp"

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace calmar {
    struct clearBuffers {
        static void init(const renderingBackend& backend);

        static u32 colorBuffer;
        static u32 depthBuffer;
        static u32 stencilBuffer;
    };
    class renderCommand {
       public:
        static void init(const renderingBackend& backend);

        static void clearBuffers(u32 buffers);
        static void clearColor(const glm::vec4& color);

       private:
        static renderingBackend mBackend;
    };
}  // namespace calmar
