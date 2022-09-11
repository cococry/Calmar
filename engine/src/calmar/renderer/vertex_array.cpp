#include "vertex_array.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/gl_vertex_array.hpp"

namespace calmar {
    const vertexArray& vertexArray::create(u32 vertexStride) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glVertexArray(vertexStride);
                break;
            default:
                break;
        }
    }

    const std::shared_ptr<vertexArray>& vertexArray::createRef(u32 vertexStride) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glVertexArray>(vertexStride);
                break;
            default:
                break;
        }
    }

    const std::unique_ptr<vertexArray>& vertexArray::createScoped(u32 vertexStride) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glVertexArray>(vertexStride);
                break;
            default:
                break;
        }
    }
}  // namespace calmar
