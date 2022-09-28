#include "window_glfw.hpp"

#include "calmar/core/asserting.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "calmar/event_system/window_events.hpp"

#include "calmar/core/application.hpp"
#include "glfw_input.hpp"

#include <stb_image.h>

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

        mCurrentFrameTime = glfwGetTime();
        mDeltaTime = (mCurrentFrameTime - mLastFrameTime);
        mFrameCounter++;
    }

    void glfwWindow::initBackend() {
        CALMAR_ASSERT_MSG(glfwInit(), "Failed to initialize GLFW.");

        glfwWindowHint(GLFW_RESIZABLE, mProps.resizable);

        mBackendHandle = glfwCreateWindow(mProps.width, mProps.height, mProps.title, mProps.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

        CALMAR_ASSERT_MSG(mBackendHandle, "Failed to create GLFW Window.");

        glfwMakeContextCurrent(mBackendHandle);

        switch (mProps.renderBackend) {
            case renderingBackend::OPENGL:
                CALMAR_ASSERT_MSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad with GLFW.");
                CALMAR_INFO("Initializing GLFW window with OpenGL rendering backend.");
                break;
            case renderingBackend::DIRECT3D:
                CALMAR_WARN("Direct3D is currently not supported as a GLFW rendering backend. Intializing GLFW window without rendering backend.");
                break;
            case renderingBackend::VULKAN:
                CALMAR_WARN("Vulkan is currently not supported as a GLFW rendering backend. Intializing GLFW window without rendering backend.");
                break;
            default:
                CALMAR_WARN("Initializing GLFW window without rendering backend.");
        }

        glfwSetWindowUserPointer(mBackendHandle, &mProps);

        glfwSetWindowSizeCallback(mBackendHandle, [](GLFWwindow* window, int width, int height) {
            windowProperties& props = *(windowProperties*)glfwGetWindowUserPointer(window);
            props.width = width;
            props.height = height;
            application::getInstance()->evDispatcher.dispatch(windowResizeEvent(width, height));
        });
        glfwSetWindowCloseCallback(mBackendHandle, [](GLFWwindow* window) {
            application::getInstance()->evDispatcher.dispatch(windowCloseEvent());
        });
        glfwSetKeyCallback(mBackendHandle, glfwInput::glfwKeyCallback);
        glfwSetMouseButtonCallback(mBackendHandle, glfwInput::glfwButtonCallback);
        glfwSetCursorPosCallback(mBackendHandle, glfwInput::glfwCursorCallback);
        glfwSetScrollCallback(mBackendHandle, glfwInput::glfwScrollCallback);

        GLFWimage images[1];
        images[0].pixels = stbi_load("../git-assets/branding_logo.png", &images[0].width, &images[0].height, 0, 4);  // rgba channels
        glfwSetWindowIcon(mBackendHandle, 1, images);
        stbi_image_free(images[0].pixels);

        glfwSetWindowPos(mBackendHandle, mProps.xpos, mProps.ypos);

        glfwSwapInterval(mProps.vsync);

        CALMAR_INFO("Created GLFW Window: '{0}' ({1}x{2}), Position on screen: {3}x{4}\t Vsync: {5} | Resizable: {6} | Fullscreen {7}",
                    mProps.title,
                    mProps.width,
                    mProps.height,
                    mProps.xpos,
                    mProps.ypos,
                    mProps.vsync ? "On" : "Off",
                    mProps.resizable ? "On" : "Off",
                    mProps.fullscreen ? "On" : "Off");
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

    void glfwWindow::setPosition(u32 xpos, u32 ypos) {
        glfwSetWindowPos(mBackendHandle, xpos, ypos);

        mProps.xpos = xpos;
        mProps.ypos = ypos;
    }

    void* glfwWindow::getBackendHandle() const {
        return mBackendHandle;
    }

    bool glfwWindow::closeRequested() const {
        return glfwWindowShouldClose(mBackendHandle);
    }

    void glfwWindow::stopTiming() {
        if (mDeltaTime >= 1.0f / 30.0f) {
            mFps = (1.0f / mDeltaTime) * mFrameCounter;
            mLastFrameTime = mCurrentFrameTime;
            mFrameCounter = 0;
        }
    }

    double glfwWindow::getDeltaTime() const {
        return mDeltaTime;
    }

    float glfwWindow::getFps() const {
        return mFps;
    }

}  // namespace calmar
