#include "pch.h"
#include "render_buffers.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/gl_render_buffers.hpp"

#include <glad/glad.h>

namespace calmar {
    u32 renderBufferFlags::dynamicDraw;
    u32 renderBufferFlags::staticDraw;

    void renderBufferFlags::init() {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                dynamicDraw = 0x88E8;
                staticDraw = 0x88E4;
                break;
            default:
                dynamicDraw = 0;
                staticDraw = 0;
                break;
        }
    }

    vertexBuffer vertexBuffer::create(float* data, u32 size, u32 bufferFlags) {
        glVertexBuffer buffer = glVertexBuffer(data, size, bufferFlags);
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return buffer;
                break;
            default:
                break;
        }
        return vertexBuffer();
    }
    vertexBuffer vertexBuffer::create(u32 size, u32 bufferFlags) {
        glVertexBuffer buffer = glVertexBuffer(size, bufferFlags);
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return buffer;
            default:
                break;
        }
        return vertexBuffer();
    }
    std::shared_ptr<vertexBuffer> vertexBuffer::createRef(float* data, u32 size, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glVertexBuffer>(data, size, bufferFlags);
                break;
            default:
                break;
        }

        return std::make_shared<vertexBuffer>();
    }
    std::shared_ptr<vertexBuffer> vertexBuffer::createRef(u32 size, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glVertexBuffer>(size, bufferFlags);
                break;
            default:
                break;
        }
        return std::make_shared<vertexBuffer>();
    }
    std::unique_ptr<vertexBuffer> vertexBuffer::createScoped(float* data, u32 size, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glVertexBuffer>(data, size, bufferFlags);
                break;
            default:
                break;
        }
        return std::make_unique<vertexBuffer>();
    }
    std::unique_ptr<vertexBuffer> vertexBuffer::createScoped(u32 size, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glVertexBuffer>(size, bufferFlags);
                break;
            default:
                break;
        }
        return std::make_unique<vertexBuffer>();
    }

    indexBuffer indexBuffer::create(u32* data, u32 count, u32 bufferFlags) {
        glIndexBuffer buffer = glIndexBuffer(data, count, bufferFlags);
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return buffer;
                break;
            default:
                break;
        }
        return indexBuffer();
    }

    std::shared_ptr<indexBuffer> indexBuffer::createRef(u32* data, u32 count, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glIndexBuffer>(data, count, bufferFlags);
                break;
            default:
                break;
        }
        return std::make_shared<indexBuffer>();
    }

    std::unique_ptr<indexBuffer> indexBuffer::createScoped(u32* data, u32 count, u32 bufferFlags) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glIndexBuffer>(data, count, bufferFlags);
                break;
            default:
                break;
        }

        return std::make_unique<indexBuffer>();
    }

}  // namespace calmar
