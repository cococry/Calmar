#include "render_command.hpp"

#include "calmar/platform/opengl/gl_render_command.hpp"

#include "calmar/core/application.hpp"

namespace calmar {
    renderingBackend renderCommand::mBackend;

    u32 clearBuffers::colorBuffer;
    u32 clearBuffers::depthBuffer;
    u32 clearBuffers::stencilBuffer;

    void clearBuffers::init() {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::NONE:
                break;
            case renderingBackend::OPENGL:
                colorBuffer = 0x00004000;
                depthBuffer = 0x00000100;
                stencilBuffer = 0x00000400;
                break;
            default:
                colorBuffer = 0;
                depthBuffer = 0;
                stencilBuffer = 0;
                break;
        }
    }

    void renderCommand::init(const renderingBackend& backend) {
        mBackend = backend;
    }

    void renderCommand::clearBuffers(u32 buffers) {
        switch (mBackend) {
            case renderingBackend::NONE:
                return;
            case renderingBackend::OPENGL:
                glRenderCommand::clearBuffers(buffers);
                break;
            default:
                break;
        }
    }

    void renderCommand::clearColor(const glm::vec4& color) {
        switch (mBackend) {
            case renderingBackend::NONE:
                return;
            case renderingBackend::OPENGL:
                glRenderCommand::clearColor(color);
                break;
            default:
                break;
        }
    }

    void renderCommand::setViewport(u32 width, u32 height) {
        switch (mBackend) {
            case renderingBackend::NONE:
                return;
            case renderingBackend::OPENGL:
                glRenderCommand::setViewport(width, height);
                break;
            default:
                break;
        }
    }

    void renderCommand::drawIndexed(u32 count) {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void renderCommand::drawIndexed(const std::shared_ptr<vertexArray>& va) {
        glDrawElements(GL_TRIANGLES, va->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void renderCommand::drawIndexed(const std::shared_ptr<vertexArray>& va, u32 count) {
        va->bind();
        u32 indexCount = count ? count : va->getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
}  // namespace calmar
