#pragma once

#include "calmar/core/application_attachment.hpp"

#include "calmar/renderer/orbit_camera.hpp"
#include "calmar/renderer/framebuffer.hpp"
#include "calmar/renderer/texture.hpp"

#include "panels/scene_hirarchy.hpp"
#include "panels/stats_panel.hpp"

#include <memory>
#include <glm/glm.hpp>

#include <filesystem>

using namespace calmar;

namespace calmarEd {
    enum class sceneState {
        PlayMode = 0,
        EditorMode
    };
    class editorAttachment : public applicationAttachment {
       public:
        virtual void init() override;

        virtual void update() override;

        virtual void handleEvents(event ev) override;

        virtual void shutdown() override;

        virtual void renderImGui() override;

        orbitCamera camera;

        sceneHirarchyPanel sceneHirarchy;
        statsPanel imguiStatsPanel;

       private:
        std::shared_ptr<framebuffer> mFramebuffer;

        std::shared_ptr<texture2d> mStopIcon, mStartIcon;

        std::shared_ptr<scene> mActiveScene, mEditorScene;

        glm::vec2 mViewportSize = glm::vec2(1.0f);
        glm::vec2 mViewportBounds[2];

        i32 mGizmoType = -1;

        sceneState mSceneState;

        bool mFirstRun = true, mViewportFocused = false;

        entity mHoveredEntity = -1;

        void renderImGuiSceneViewport();

        void renderPlayButton();

        void handleImGuiDockspace();

        void renderGizmos();

        void handleInput();

        void newScene();

        void openScene();

        void openScene(const std::filesystem::path& path);

        void saveSceneAs();

        void saveScene();

        void startRuntimeScene();

        void stopRuntimeScene();

        std::filesystem::path mScenePath;
    };
}  // namespace calmarEd
