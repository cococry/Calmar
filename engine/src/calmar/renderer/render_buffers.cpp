#include "render_buffers.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/gl_render_buffers.hpp"

namespace calmar {
    u32 renderBufferFlags::dynamicDraw;
    u32 renderBufferFlags::staticDraw;

    void renderBufferFlags::init(const renderingBackend& backend) {
        switch (backend) {
            case renderingBackend::OPENGL:
                dynamicDraw = 0x88E4;
                staticDraw = 0x88E5;
            default:
                dynamicDraw = 0;
                staticDraw = 0;
                break;
        }
    }

    const vertexBuffer& vertexBuffer::create(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glVertexBuffer(data, size, bufferFlags);
                break;
            default:
                break;
        }
    }
    const vertexBuffer& vertexBuffer::create(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glVertexBuffer(size, bufferFlags);
                break;
            default:
                break;
        }
    }
    const std::shared_ptr<vertexBuffer>& vertexBuffer::createRef(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glVertexBuffer>(data, size, bufferFlags);
                break;
            default:
                break;
        }
    }
    const std::shared_ptr<vertexBuffer>& vertexBuffer::createRef(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glVertexBuffer>(size, bufferFlags);
                break;
            default:
                break;
        }
    }
    const std::unique_ptr<vertexBuffer>& vertexBuffer::createScoped(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glVertexBuffer>(data, size, bufferFlags);
                break;
            default:
                break;
        }
    }
    const std::unique_ptr<vertexBuffer>& vertexBuffer::createScoped(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glVertexBuffer>(size, bufferFlags);
                break;
            default:
                break;
        }
    }

    const indexBuffer& indexBuffer::create(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glIndexBuffer(data, count, bufferFlags);
                break;
            default:
                break;
        }
    }

    const std::shared_ptr<indexBuffer>& indexBuffer::createRef(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glIndexBuffer>(data, count, bufferFlags);
                break;
            default:
                break;
        }
    }

    const std::unique_ptr<indexBuffer>& indexBuffer::createScoped(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glIndexBuffer>(data, count, bufferFlags);
                break;
            default:
                break;
        }
    }

}  // namespace calmar
