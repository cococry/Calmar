#pragma once

#include <calmar/core/defines.hpp>
#include <calmar/core/application_attachment.hpp>
#include <calmar/renderer/texture.hpp>
#include <calmar/ecs/scene.hpp>

#include <memory>
#include <string>
#include <glm/glm.hpp>

using namespace calmar;

namespace calmarEd {
    class sceneHirarchyPanel {
       public:
        void init();

        void update();

        void renderImGui();

       private:
        void renderImGuiEntityNode(entity entty);

        void renderImGuiEntityComponents(entity entty);

        void renderImGuiVec3Slider(const std::string& label, glm::vec3& vec, entity entty, float resetValue = 0.0f);

        std::shared_ptr<scene> mScene;
        std::shared_ptr<texture2d> mDefaultTexture;

        entity mSelectedEntity = -1;
        entity mDeletedEntity = 1;
    };
}  // namespace calmarEd
