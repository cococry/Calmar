#include "pch.h"
#include "imgui_handler.hpp"

#include <calmar/core/application.hpp>
#include <calmar/event_system/mouse_events.hpp>
#include <calmar/event_system/key_events.hpp>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include <GLFW/glfw3.h>

namespace calmar {
    imGuiHandler::imGuiHandler() {
    }

    imGuiHandler::~imGuiHandler() {
    }

    void imGuiHandler::init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.Fonts->AddFontFromFileTTF("../editor/assets/fonts/roboto/Roboto-Bold.ttf", 16.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("../editor/assets/fonts/roboto/Roboto-Bold.ttf", 16.0f);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImColor(41, 41, 41);

        // Headers
        style.Colors[ImGuiCol_Header] = ImColor(62, 62, 62);
        style.Colors[ImGuiCol_HeaderHovered] = ImColor(67, 67, 67);
        style.Colors[ImGuiCol_HeaderActive] = ImColor(59, 59, 59);

        // Buttons
        style.Colors[ImGuiCol_Button] = ImColor(55, 55, 55);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(62, 62, 62);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(50, 50, 50);

        // Frame BG
        style.Colors[ImGuiCol_FrameBg] = ImColor(67, 67, 67);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(72, 72, 72);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(62, 62, 62);

        style.Colors[ImGuiCol_MenuBarBg] = ImColor(55, 55, 55);
        style.Colors[ImGuiCol_PopupBg] = ImColor(60, 60, 60);

        // Tabs
        style.Colors[ImGuiCol_Tab] = ImColor(55, 55, 55);
        style.Colors[ImGuiCol_TabHovered] = ImColor(62, 62, 62);
        style.Colors[ImGuiCol_TabActive] = ImColor(50, 50, 50);
        style.Colors[ImGuiCol_TabUnfocused] = ImColor(52, 52, 52);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(50, 50, 50);

        // Title
        style.Colors[ImGuiCol_TitleBg] = ImColor(52, 52, 52);
        style.Colors[ImGuiCol_TitleBgActive] = ImColor(58, 58, 58);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(60, 60, 60);

        GLFWwindow* window = static_cast<GLFWwindow*>(application::getInstance()->display->getBackendHandle());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void imGuiHandler::shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void imGuiHandler::renderImGui() {
    }
    void imGuiHandler::beginImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }
    void imGuiHandler::endImGui() {
        ImGuiIO& io = ImGui::GetIO();
        std::shared_ptr<window> window = application::getInstance()->display;
        io.DisplaySize = ImVec2((float)window->getProperties().width, (float)window->getProperties().height);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* currentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(currentContext);
        }
    }

}  // namespace calmar
