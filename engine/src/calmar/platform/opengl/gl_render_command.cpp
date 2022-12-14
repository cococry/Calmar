#include "pch.h"
#include "gl_render_command.hpp"

#include <glad/glad.h>

namespace calmar {
    void glRenderCommand::clearBuffers(u32 buffers) {
        glClear(buffers);
    }
    void glRenderCommand::clearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void glRenderCommand::setViewport(u32 width, u32 height) {
        glViewport(0, 0, width, height);
    }

    void glRenderCommand::drawIndexed(u32 count) {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
    void glRenderCommand::drawIndexed(const std::shared_ptr<vertexArray>& va) {
        glDrawElements(GL_TRIANGLES, va->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }
}  // namespace calmar
