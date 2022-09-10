#include "renderer.hpp"

#include "render_command.hpp"

#include "render_buffers.hpp"

namespace calmar {
    void renderer::initSubsystems(const renderingBackend& backend) {
        clearBuffers::init(backend);
        renderCommand::init(backend);
        renderBufferFlags::init(backend);
    }

}  // namespace calmar
