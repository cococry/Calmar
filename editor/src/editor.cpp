#include "editor.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/core/application.hpp"

#include "calmar/event_system/mouse_events.hpp"

#include "calmar/renderer/batch_renderer_2d.hpp"
#include "calmar/renderer/render_command.hpp"

namespace calmarEd {

    void editorAttachment::init() {
        application::getInstance()->evDispatcher.listen(mouseScrolledEvent::evType, std::bind(&editorAttachment::handleEvents, this, std::placeholders::_1));

        mCppLogoTexture = texture2d::createRef("../engine/assets/textures/cpplogo.png");
        mCalmarLogoTexture = texture2d::createRef("../engine/assets/textures/calmarlogo.png");
        mFoliageTexture = texture2d::createRef("../engine/assets/textures/foliage.jpg");
        mEditorCamera = orbitCamera(cameraProperties());
    }

    void editorAttachment::update() {
        renderCommand::clearBuffers(clearBuffers::colorBuffer);
        renderCommand::clearColor({0.2f, 0.3f, 0.8f, 1.0f});

        batchRenderer2d::beginRender(mEditorCamera);
        for (u32 y = 0; y < 25; y++) {
            for (u32 x = 0; x < 25; x++) {
                batchRenderer2d::renderQuad(glm::vec2(x, y), glm::vec2(1.0f), x % 2 == 0 ? mFoliageTexture : mCalmarLogoTexture);
            }
        }
        batchRenderer2d::endRender();
        mEditorCamera.update();
    }

    void editorAttachment::shutdown() {
    }

    void editorAttachment::handleEvents(const event& ev) {
        if (ev.type() == windowResizeEvent::evType) {
            const windowResizeEvent& resizeEvent = static_cast<const windowResizeEvent&>(ev);
            mEditorCamera.resize(resizeEvent.getWidth(), resizeEvent.getHeight());
        }
        mEditorCamera.handleEvents(ev);
    }
}  // namespace calmarEd
