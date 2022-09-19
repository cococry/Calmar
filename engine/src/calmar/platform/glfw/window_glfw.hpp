#ifndef CALMAR_GLFW_WINDOWING_INCLUDED
#define CALMAR_GLFW_WINDOWING_INCLUDED

#include "calmar/core/defines.hpp"
#include "calmar/core/window.hpp"

struct GLFWwindow;

namespace calmar {
    class glfwWindow : public window {
       public:
        glfwWindow();

        glfwWindow(const windowProperties& props);

        ~glfwWindow();

        virtual void update(bool updateAbsoluteTime = false) override;

        virtual void setWidth(u32 width) override;

        virtual void setHeight(u32 height) override;

        virtual void setTitle(const char* title) override;

        virtual void setVsync(bool vsync) override;

        virtual void setResizable(bool resizable) override;

        virtual void setPosition(u32 xpos, u32 ypos) override;

        virtual void* getBackendHandle() const override;

        virtual bool closeRequested() const override;

        virtual double getAbsoluteTime() const override;

       private:
        GLFWwindow* mBackendHandle;

        double mAbsoluteTime = 0.0;

       private:
        virtual void
        initBackend() override;

        virtual void shutdownBackend() override;
    };
}  // namespace calmar

#endif