#include "renderer.hpp"

#include "render_command.hpp"

#include "render_buffers.hpp"

#include "render_data_types.hpp"

namespace calmar {
    void renderer::initSubsystems(const renderingBackend& backend) {
        clearBuffers::init();
        renderBufferFlags::init();
        renderDataTypes::init();
        renderCommand::init(backend);
    }

}  // namespace calmar
