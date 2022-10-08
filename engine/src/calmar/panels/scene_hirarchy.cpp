#include "scene_hirarchy.hpp"

#include <imgui.h>

namespace calmar {
    void sceneHirarchyPanel::init() {
    }
    void sceneHirarchyPanel::update() {
    }
    void sceneHirarchyPanel::renderImGui() {
        ImGui::Begin("Hello");
        ImGui::Text("This is working");
        ImGui::End();
    }
}  // namespace calmar
