#pragma once

#include <calmar/core/defines.hpp>
#include <calmar/core/application_attachment.hpp>

#include <calmar/ecs/scene.hpp>

#include <memory>

using namespace calmar;

namespace calmarEd {
    class sceneHirarchyPanel {
       public:
        void init();

        void update();

        void renderImGui();

       private:
        std::shared_ptr<scene> mScene;

        entity mSelectedEntity = -1;
    };
}  // namespace calmarEd
