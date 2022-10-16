#include "framebuffer.hpp"

#include "calmar/platform/opengl/gl_frame_buffer.hpp"

#include "calmar/core/application.hpp"

namespace calmar {
    framebuffer framebuffer::create(const framebufferProperties& props) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glFrameBuffer(props);
                break;
            default:
                return framebuffer();
                break;
        }
    }
    std::unique_ptr<framebuffer> framebuffer::createScoped(const framebufferProperties& props) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glFrameBuffer>(props);
                break;
            default:
                return std::make_unique<framebuffer>();
                break;
        }
    }
    std::shared_ptr<framebuffer> framebuffer::createRef(const framebufferProperties& props) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glFrameBuffer>(props);
                break;
            default:
                return std::make_shared<framebuffer>();
                break;
        }
    }
}  // namespace calmar
