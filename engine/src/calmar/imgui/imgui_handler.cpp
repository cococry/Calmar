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

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImColor(13, 18, 20);

        // Headers
        style.Colors[ImGuiCol_Header] = ImColor(23, 28, 31);
        style.Colors[ImGuiCol_HeaderHovered] = ImColor(30, 35, 37);
        style.Colors[ImGuiCol_HeaderActive] = ImColor(19, 25, 28);

        // Buttons
        style.Colors[ImGuiCol_Button] = ImColor(58, 59, 69);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(68, 69, 79);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(48, 49, 59);

        // Frame BG
        style.Colors[ImGuiCol_FrameBg] = ImColor(39, 47, 51);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(47, 53, 59);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(32, 42, 55);

        style.Colors[ImGuiCol_MenuBarBg] = ImColor(34, 43, 47);
        style.Colors[ImGuiCol_PopupBg] = ImColor(19, 24, 27);

        // Tabs
        style.Colors[ImGuiCol_Tab] = ImColor(37, 47, 53);
        style.Colors[ImGuiCol_TabHovered] = ImColor(45, 53, 58);
        style.Colors[ImGuiCol_TabActive] = ImColor(30, 40, 45);
        style.Colors[ImGuiCol_TabUnfocused] = ImColor(27, 38, 43);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(23, 34, 56);

        // Title
        style.Colors[ImGuiCol_TitleBg] = ImColor(16, 22, 26);
        style.Colors[ImGuiCol_TitleBgActive] = ImColor(20, 26, 30);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(11, 16, 20);

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
