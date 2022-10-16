#include "editor.hpp"

#include "calmar/core/application.hpp"

#include "calmar/renderer/render_command.hpp"

#include <imgui.h>

namespace calmarEd {
    void editorAttachment::init() {
        camera = orbitCamera(cameraProperties());

        sceneHirarchy = sceneHirarchyPanel();
        sceneHirarchy.init();
        imguiStatsPanel.init();

        framebufferProperties framebufferProps;
        framebufferProps.attachments = {framebufferTextureFormat::RGBA8, framebufferTextureFormat::Depth};
        framebufferProps.width = 1920;
        framebufferProps.height = 1080;
        mFramebuffer = framebuffer::createRef(framebufferProps);
    }

    void editorAttachment::update() {
        mFramebuffer->bind();
        camera.update();
        renderCommand::clearBuffers(clearBuffers::colorBuffer | clearBuffers::depthBuffer);
        renderCommand::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
        sceneHirarchy.update();
        mFramebuffer->unbind();

        // for imgui
        renderCommand::clearBuffers(clearBuffers::colorBuffer | clearBuffers::depthBuffer);
        renderCommand::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
    }

    void editorAttachment::handleEvents(const event& ev) {
        camera.handleEvents(ev);
    }

    void editorAttachment::shutdown() {
    }

    void editorAttachment::renderImGui() {
        handleImGuiDockspace();
        renderImGuiSceneViewport();
        sceneHirarchy.renderImGui();
        imguiStatsPanel.renderImGui();
    }

    void editorAttachment::renderImGuiSceneViewport() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        ImVec2 panelSize = ImGui::GetContentRegionAvail();

        if (mViewportSize != *((glm::vec2*)&panelSize)) {
            mFramebuffer->resize((u32)panelSize.x, (u32)panelSize.y);
            mViewportSize = {panelSize.x, panelSize.y};
            camera.resize(panelSize.x, panelSize.y);
        }
        render_id texId = mFramebuffer->getColorAttachmentId();
        ImGui::Image((void*)(uintptr_t)texId, ImVec2{mViewportSize.x, mViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
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
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
}  // namespace calmarEd
