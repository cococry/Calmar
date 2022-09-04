#include "render_command.hpp"

#include "calmar/platform/opengl/gl_render_command.hpp"

namespace calmar {
    renderingBackend renderCommand::mBackend;

    u32 clearBuffers::colorBuffer;
    u32 clearBuffers::depthBuffer;
    u32 clearBuffers::stencilBuffer;

    void clearBuffers::init(const renderingBackend& backend) {
        switch (backend) {
            case renderingBackend::NONE:
                break;
            case renderingBackend::OPENGL:
                colorBuffer = 0x00004000;
                depthBuffer = 0x00000100;
                stencilBuffer = 0x00000400;
                break;
            default:
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

}  // namespace calmar
