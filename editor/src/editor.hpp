#pragma once

#include "calmar/core/application_attachment.hpp"

#include "calmar/renderer/orbit_camera.hpp"

#include "panels/scene_hirarchy.hpp"
#include "panels/stats_panel.hpp"

#include <memory>

using namespace calmar;

namespace calmarEd {
    class editorAttachment : public applicationAttachment {
       public:
        virtual void init() override;

        virtual void update() override;

        virtual void handleEvents(const event& ev) override;

        virtual void shutdown() override;

        virtual void renderImGui() override;

        orbitCamera camera;

        sceneHirarchyPanel sceneHirarchy;
        statsPanel imguiStatsPanel;

       private:
    };
}  // namespace calmarEd
