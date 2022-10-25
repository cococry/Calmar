#include "editor.hpp"

#include <calmar/core/application.hpp>
#include <calmar/renderer/render_command.hpp>
#include <calmar/renderer/resource_handler.hpp>
#include <calmar/input/input.hpp>
#include <calmar/input/key_codes.hpp>
#include <calmar/input/mouse_codes.hpp>
#include <calmar/event_system/mouse_events.hpp>
#include <calmar/core/util.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <calmar/event_system/window_events.hpp>

#include <calmar/ecs/scene_serializer.hpp>

#include <ImGuizmo.h>

namespace calmarEd {
    void editorAttachment::init() {
        camera = orbitCamera(cameraProperties());

        imguiStatsPanel.init();

        framebufferProperties framebufferProps;
        framebufferProps.attachments = {framebufferTextureFormat::RGBA8, framebufferTextureFormat::SHADER_RED_INT, framebufferTextureFormat::Depth};
        framebufferProps.width = 1280;
        framebufferProps.height = 720;
        mFramebuffer = framebuffer::createRef(framebufferProps);

        mStopIcon = resourceHandler::createTexture("../editor/assets/icons/stop-icon.png");
        mStartIcon = resourceHandler::createTexture("../editor/assets/icons/play-icon.png");

        mActiveScene = ECS.registerSystem<scene>();
        mActiveScene->init();
        mEditorScene = mActiveScene;
        mEditorScene->init();
        mSceneState = sceneState::EditorMode;

        sceneHirarchy = sceneHirarchyPanel();
        sceneHirarchy.init(mEditorScene);
    }

    void editorAttachment::update() {
        mFramebuffer->bind();
        renderCommand::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
        renderCommand::clearBuffers(clearBuffers::colorBuffer | clearBuffers::depthBuffer);
        mFramebuffer->clearAttachment(1, -1);

        if (mSceneState == sceneState::EditorMode) {
            mActiveScene->update();
        } else {
            mActiveScene->updateRuntime();
        }

        auto [imguiMouseX, imguiMouseY] = ImGui::GetMousePos();
        imguiMouseX -= mViewportBounds[0].x;
        imguiMouseY -= mViewportBounds[0].y;
        glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
        imguiMouseY = viewportSize.y - imguiMouseY;

        i32 mouseX = (i32)imguiMouseX;
        i32 mouseY = (i32)imguiMouseY - 20;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            i32 pixelData = mFramebuffer->readPixel(1, mouseX, mouseY);
            mHoveredEntity = pixelData;
        }
        if (mHoveredEntity == -1 && mViewportFocused && input::mouseButtonWentDown(button::Left) && !ImGuizmo::IsOver()) {
            sceneHirarchy.setSelectedEntity(-1);
        }

        mFramebuffer->unbind();
        // for imgui
        renderCommand::clearBuffers(clearBuffers::colorBuffer | clearBuffers::depthBuffer);
        renderCommand::clearColor({0.1f, 0.1f, 0.1f, 1.0f});

        if (mViewportFocused) {
            handleInput();
            camera.update();
            sceneHirarchy.update();
        }
    }

    void editorAttachment::handleEvents(event ev) {
        camera.handleEvents(ev);

        if (COMPARE_EVENTS(ev, windowResizeEvent)) {
            mActiveScene->handleResize(application::getInstance()->display->getProperties().width, application::getInstance()->display->getProperties().height);
        }
    }

    void editorAttachment::shutdown() {
    }

    void editorAttachment::renderImGui() {
        handleImGuiDockspace();
        renderImGuiSceneViewport();
        renderGizmos();
        renderPlayButton();
        sceneHirarchy.renderImGui();
        imguiStatsPanel.renderImGui();
    }

    void editorAttachment::renderImGuiSceneViewport() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        ImVec2 viewportOffset = ImGui::GetCursorPos();
        mViewportFocused = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

        ImVec2 panelSize = ImGui::GetContentRegionAvail();

        if (mFirstRun) {
            mActiveScene->handleResize(panelSize.x, panelSize.y);
            mFirstRun = false;
        }
        mActiveScene->handleResize(panelSize.x, panelSize.y);
        if (mViewportSize != *((glm::vec2*)&panelSize)) {
            mFramebuffer->resize((u32)panelSize.x, (u32)panelSize.y);
            mViewportSize = {panelSize.x, panelSize.y};
            camera.resize(panelSize.x, panelSize.y);
            mActiveScene->handleResize(panelSize.x, panelSize.y);
        }
        render_id texId = mFramebuffer->getColorAttachmentId(0);
        ImGui::Image((void*)(uintptr_t)texId, ImVec2{mViewportSize.x, mViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = {ImGui::GetWindowPos().x + viewportOffset.x, ImGui::GetWindowPos().y + viewportOffset.y};
        ImVec2 maxBound = {minBound.x + windowSize.x, minBound.y + windowSize.y};
        mViewportBounds[0] = {minBound.x, minBound.y};
        mViewportBounds[1] = {maxBound.x, maxBound.y};
    }

    void editorAttachment::renderPlayButton() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = 32.0f;
        std::shared_ptr<texture2d> icon = mSceneState == sceneState::EditorMode ? mStartIcon : mStopIcon;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getId()), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0)) {
            if (mSceneState == sceneState::EditorMode) {
                mGizmoType = -1;
                mSceneState = sceneState::PlayMode;
                startRuntimeScene();
            } else if (mSceneState == sceneState::PlayMode) {
                mSceneState = sceneState::EditorMode;
                stopRuntimeScene();
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void editorAttachment::handleImGuiDockspace() {
        static bool dockspace_open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
                    application::getInstance()->close();
                }
                if (ImGui::MenuItem("New Scene")) {
                    newScene();
                }
                if (ImGui::MenuItem("Save Scene")) {
                    saveScene();
                }
                if (ImGui::MenuItem("Save Scene As")) {
                    saveSceneAs();
                }
                if (ImGui::MenuItem("Open Scene")) {
                    openScene();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void editorAttachment::renderGizmos() {
        entity selectedEntity = sceneHirarchy.getSelectedEntity();
        if (selectedEntity != -1 && mGizmoType != -1 && !input::isKeyDown(key::LeftAlt)) {
            if (ECS.hasComponent<cameraComponent>(selectedEntity)) {
                if (mGizmoType == ImGuizmo::OPERATION::SCALE) {
                    ImGui::End();
                    ImGui::PopStyleVar();

                    ImGui::End();
                    return;
                }
            }

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            const glm::mat4& cameraProjection = camera.getData().projMatrix;
            glm::mat4 cameraView = camera.getData().viewMatrix;

            auto& transformComp = ECS.getComponent<transformComponent>(selectedEntity);
            glm::mat4 transform = transformComp.getTransform();

            bool snapped = input::isKeyDown(key::LeftControl);
            float snapValue = 1.0f;
            if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)mGizmoType,
                                 ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snapped ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                math::linalg::decomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - transformComp.rotation;
                transformComp.position = translation;
                transformComp.rotation += deltaRotation;
                transformComp.scale = scale;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void editorAttachment::handleInput() {
        if (input::isKeyDown(key::Q) || input::isKeyDown(key::Escape)) {
            mGizmoType = -1;
        } else if (input::isKeyDown(key::W)) {
            if (mSceneState != sceneState::PlayMode) {
                mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
        } else if (input::isKeyDown(key::R)) {
            if (mSceneState != sceneState::PlayMode) {
                mGizmoType = ImGuizmo::OPERATION::ROTATE;
            }
        } else if (input::isKeyDown(key::S)) {
            if (mSceneState != sceneState::PlayMode) {
                mGizmoType = ImGuizmo::OPERATION::SCALE;
            }
        }
        if (input::mouseButtonWentDown(button::Left) && mHoveredEntity != -1 && mViewportFocused && !ImGuizmo::IsOver()) {
            sceneHirarchy.setSelectedEntity(mHoveredEntity);
        }
    }

    void editorAttachment::newScene() {
        mGizmoType = -1;
        mActiveScene = ECS.registerSystem<scene>();
        mActiveScene->handleResize(mViewportSize.x, mViewportSize.y);
        sceneHirarchy.setScene(mActiveScene);

        mScenePath = std::filesystem::path();
    }

    void editorAttachment::openScene() {
        std::string filepath = platform::fileDialogs::openFile("Calmar Scene (*.clmscene)\0*.clmscene\0");

        if (!filepath.empty()) {
            openScene(filepath);
        }
    }

    void editorAttachment::openScene(const std::filesystem::path& path) {
        if (path.extension().string() != ".clmscene") {
            CALMAR_ERROR("Failed to scene file '{0}' - not a .clmscene file", path.filename().string());
            return;
        }
        std::shared_ptr<scene> newScene = ECS.registerSystem<scene>();
        newScene->init();
        mEditorScene = newScene;
        mEditorScene->handleResize(mViewportSize.x, mViewportSize.y);
        sceneSerialzer serialzer(newScene);
        if (serialzer.deserialize(path.string())) {
            mActiveScene = mEditorScene;
            sceneHirarchy.setScene(mActiveScene);
            mScenePath = path;
        }
    }

    void editorAttachment::saveSceneAs() {
        std::string filepath = platform::fileDialogs::saveFile("Calmar Scene (*.clmscene)\0*.clmscene\0");
        if (!filepath.empty()) {
            sceneSerialzer serializer(mActiveScene);
            serializer.serialize(filepath);

            mScenePath = filepath;
        }
    }

    void editorAttachment::saveScene() {
        if (!mScenePath.empty()) {
            sceneSerialzer serializer(mActiveScene);
            serializer.serialize(mScenePath.string());
        } else {
            saveSceneAs();
        }
    }

    void editorAttachment::startRuntimeScene() {
        mActiveScene = scene::copy(mEditorScene);
    }

    void editorAttachment::stopRuntimeScene() {
        mActiveScene = mEditorScene;
    }
}  // namespace calmarEd
