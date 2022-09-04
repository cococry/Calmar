#include "renderer.hpp"

#include "render_command.hpp"

namespace calmar {
    void renderer::initSubsystems(const renderingBackend& backend) {
        clearBuffers::init(backend);
        renderCommand::init(backend);
    }

}  // namespace calmar
