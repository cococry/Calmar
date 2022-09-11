#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/renderer/vertex_array.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace calmar {
    class glRenderCommand {
       public:
        static void clearBuffers(u32 buffers);

        static void clearColor(const glm::vec4& color);

        static void setViewport(u32 width, u32 height);

        static void drawIndexed(u32 count);

        static void drawIndexed(const std::shared_ptr<vertexArray>& va);

       private:
    };
}  // namespace calmar
