#include "gl_render_command.hpp"

#include <glad/glad.h>

namespace calmar {
    void glRenderCommand::clearBuffers(u32 buffers) {
        glClear(buffers);
    }
    void glRenderCommand::clearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }
}  // namespace calmar
