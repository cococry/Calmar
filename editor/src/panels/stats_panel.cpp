#include "stats_panel.hpp"

#include <imgui.h>

#include "calmar/core/application.hpp"
#include "calmar/renderer/batch_renderer_2d.hpp"

namespace calmarEd {
    void statsPanel::init() {
    }

    void statsPanel::renderImGui() {
        ImGui::Begin("Statistics");
        ImGui::Text("FPS Count: %.2f", application::getInstance()->display->getFps());
        ImGui::Text("Rendererd quads: %i", batchRenderer2d::getStats().numberOfQuads);
        ImGui::Text("Draw Calls: %i", batchRenderer2d::getStats().drawCalls);
        ImGui::End();
    }
}  // namespace calmarEd
