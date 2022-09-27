#ifndef CALMAR_WINDOWS_WINDOW_INCLUDED
#define CALMAR_WINDOWS_WINDOW_INCLUDED

#include <Windows.h>
#include <windowsx.h>

#include "calmar/core/window.hpp"

#include <chrono>

namespace calmar {
    struct windowsHandle {
        HINSTANCE instance;
        HWND window;
    };
    class windowsWindow : public window {
       public:
        windowsWindow();

        windowsWindow(const windowProperties& props);

        ~windowsWindow();

        virtual void update() override;

        virtual void setWidth(u32 width) override;

        virtual void setHeight(u32 height) override;

        virtual void setTitle(const char* title) override;

        virtual void setVsync(bool vsync) override;

        virtual void setResizable(bool resizable) override;

        virtual void setPosition(u32 xpos, u32 ypos) override;

        virtual void* getBackendHandle() const override;

        virtual bool closeRequested() const override;

        virtual double getDeltaTime() const override;

        virtual void stopTiming() override;

       private:
        virtual void initBackend() override;

        virtual void shutdownBackend() override;

        bool updateMessages() const;

       private:
        double getMillis();

        windowsHandle* mBackendHandle;

        double mDeltaTime = 0.0f;
        std::chrono::steady_clock::time_point mStartFrameTime;
        std::chrono::steady_clock::time_point mEndFrameTime;

        u32 mWindowStyle = 0;

        HGLRC mRenderContext;
        HDC mDeviceContext;
    };

    LRESULT CALLBACK updateMessagesWindowsWindow(HWND window, u32 msg, WPARAM wParam, LPARAM lParam);

}  // namespace calmar

#endif  // CALMAR_WINDOWS_WINDOW_INCLUDED
