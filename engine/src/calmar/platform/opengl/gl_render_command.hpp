#pragma once

#include "calmar/core/defines.hpp"

#include <glm/glm.hpp>

namespace calmar {
    class glRenderCommand {
       public:
        static void clearBuffers(u32 buffers);
        static void clearColor(const glm::vec4& color);

        static void setViewport(u32 width, u32 height);

       private:
    };
}  // namespace calmar
