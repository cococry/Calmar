#ifndef CALMAR_GLFW_WINDOWING_INCLUDED
#define CALMAR_GLFW_WINDOWING_INCLUDED

#include "core/defines.hpp"
#include "core/window.hpp"

struct GLFWwindow;

namespace calmar {
    class glfwWindow : public window {
       public:
        glfwWindow();

        glfwWindow(const windowProperties& props);

        ~glfwWindow();

        virtual void update() override;

        virtual void setWidth(u32 width) override;

        virtual void setHeight(u32 height) override;

        virtual void setTitle(const char* title) override;

        virtual void setVsync(bool vsync) override;

        virtual void setResizable(bool resizable) override;

        virtual void* getBackendHandle() const override;

        virtual bool closeRequested() const override;

       private:
        GLFWwindow* mBackendHandle;

        virtual void initBackend() override;

        virtual void shutdownBackend() override;
    };
}  // namespace calmar

#endif