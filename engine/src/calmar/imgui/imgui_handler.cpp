#include "imgui_handler.hpp"

#include "calmar/core/application.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
