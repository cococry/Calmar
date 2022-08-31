#include "window_glfw.hpp"

#include "core/asserting.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace calmar {
    glfwWindow::glfwWindow() {
        CALMAR_ASSERT_MSG(glfwInit(), "Failed to initialize GLFW.");
    }
    glfwWindow::glfwWindow(const windowProperties& props) {
        mProps = props;

        initBackend();
    }
    glfwWindow::~glfwWindow() {
        shutdownBackend();
    }

    void glfwWindow::update() {
        glfwPollEvents();
        glfwSwapBuffers(mBackendHandle);
    }

    void glfwWindow::initBackend() {
        CALMAR_ASSERT_MSG(glfwInit(), "Failed to initialize GLFW.");

        glfwWindowHint(GLFW_RESIZABLE, mProps.resizable);

        mBackendHandle = glfwCreateWindow(mProps.width, mProps.height, mProps.title, nullptr, nullptr);

        CALMAR_ASSERT_MSG(mBackendHandle, "Failed to create GLFW Window.");

        glfwMakeContextCurrent(mBackendHandle);

        CALMAR_ASSERT_MSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad.");

        glfwSwapInterval(mProps.vsync);

        CALMAR_INFO("Created GLFW Window: '{0}' ({1}x{2})\t Vsync: {3} | Resizable: {4}",
                    mProps.title, mProps.width, mProps.height,
                    mProps.vsync ? "On" : "Off", mProps.resizable ? "On" : "Off");
    }

    void glfwWindow::shutdownBackend() {
        glfwDestroyWindow(mBackendHandle);
        glfwTerminate();
    }

    void glfwWindow::setWidth(u32 width) {
        glfwSetWindowSize(mBackendHandle, width, mProps.height);
        mProps.width = width;
    }

    void glfwWindow::setHeight(u32 height) {
        glfwSetWindowSize(mBackendHandle, mProps.width, height);
        mProps.height = height;
    }

    void glfwWindow::setTitle(const char* title) {
        glfwSetWindowTitle(mBackendHandle, title);
        mProps.title = title;
    }

    void glfwWindow::setVsync(bool vsync) {
        glfwSwapInterval(vsync);

        mProps.vsync = vsync;
    }

    void glfwWindow::setResizable(bool resizable) {
        if (mBackendHandle)
            CALMAR_WARN("Cannot set resize attribute in GLFW Window after window creation.");
        else
            glfwWindowHint(GLFW_RESIZABLE, resizable);
    }

    void* glfwWindow::getBackendHandle() const {
        return mBackendHandle;
    }

    bool glfwWindow::closeRequested() const {
        return glfwWindowShouldClose(mBackendHandle);
    }

}  // namespace calmar
