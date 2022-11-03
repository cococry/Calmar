#pragma once

#include "calmar/core/window.hpp"

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

        virtual void setPosition(u32 xpos, u32 ypos) override;

        virtual void* getBackendHandle() const override;

        virtual bool closeRequested() const override;

        virtual float getDeltaTime() const override;

        virtual void startTiming() override;
        
        virtual void stopTiming() override;

        virtual float getFps() const override;

        virtual void initRenderBackend() override;

       private:
        GLFWwindow* mBackendHandle;

        float mDeltaTime = 0.0;
        float mLastFrameTime = 0.0f;
        float mCurrentFrameTime = 0.0f;
        u32 mFrameCounter = 0;
        float mFps = 0;

        virtual void initBackend() override;

        virtual void shutdownBackend() override;
    };
}  // namespace calmar