#include "editor.hpp"

#include <imgui.h>

namespace calmarEd {
    void editorAttachment::init() {
        camera = orbitCamera(cameraProperties());

        sceneHirarchy = sceneHirarchyPanel();
        sceneHirarchy.init();
        imguiStatsPanel.init();
    }

    void editorAttachment::update() {
        camera.update();
        sceneHirarchy.update();
    }

    void editorAttachment::handleEvents(const event& ev) {
        camera.handleEvents(ev);
    }

    void editorAttachment::shutdown() {
    }

    void editorAttachment::renderImGui() {
        sceneHirarchy.renderImGui();
        imguiStatsPanel.renderImGui();
    }
}  // namespace calmarEd
