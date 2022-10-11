#include "scene_hirarchy.hpp"

#include <calmar/core/application.hpp>

#include <imgui.h>

namespace calmarEd {
    void sceneHirarchyPanel::init() {
        mScene = ECS.registerSystem<scene>();

        mScene->init();
    }
    void sceneHirarchyPanel::update() {
    }
    void sceneHirarchyPanel::renderImGui() {
        ImGui::Begin("Scene Hirarchy");
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Entity")) {
                entity entty = ECS.createEntity();
                ECS.addComponent(entty, transformComponent());
                mSelectedEntity = entty;
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }
}  // namespace calmarEd
