#include "pch.h"
#include "renderer.hpp"

#include "render_command.hpp"
#include "render_buffers.hpp"
#include "render_data_types.hpp"
#include "batch_renderer_2d.hpp"

#include "calmar/platform/opengl/gl_rendering.hpp"
#include "calmar/core/application.hpp"

namespace calmar {
    void renderer::initSubsystems(const renderingBackend& backend) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                glRendering::init();
                break;

            default:
                break;
        }
        clearBuffers::init();
        renderBufferFlags::init();
        renderDataTypes::init();
        renderCommand::init(backend);
        batchRenderer2d::init();
    }

}  // namespace calmar
