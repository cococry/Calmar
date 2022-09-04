#include "window_windows.hpp"

#include "calmar/core/asserting.hpp"

#include "calmar/core/global_state.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/wgl_load.hpp"

#include "calmar/event_system/window_events.hpp"

#include "windows_input.hpp"

#include <glad/glad_wgl.h>
namespace calmar {
    windowsWindow::windowsWindow() {
    }

    windowsWindow::windowsWindow(const windowProperties& props) {
        mProps = props;

        initBackend();
    }

    windowsWindow::~windowsWindow() {
        shutdownBackend();
    }

    void windowsWindow::update() {
        updateMessages();

        wglSwapIntervalEXT(mProps.vsync);
        wglSwapLayerBuffers(mDeviceContext, WGL_SWAP_MAIN_PLANE);
    }

    void windowsWindow::setWidth(u32 width) {
        u32 windowX = 0, windowY = 0;
        RECT rect = {NULL};
        if (GetWindowRect(mBackendHandle->window, &rect)) {
            windowX = rect.left;
            windowY = rect.top;
        }
        RECT* tileClientArea = 0;
        GetClientRect(mBackendHandle->window, tileClientArea);

        u32 height = tileClientArea->bottom - tileClientArea->top;

        SetWindowPos(mBackendHandle->window, mBackendHandle->window, windowX, windowY, width, height, SWP_NOREPOSITION);

        mProps.width = width;
    }

    void windowsWindow::setHeight(u32 height) {
        u32 windowX = 0, windowY = 0;
        RECT rect = {NULL};
        if (GetWindowRect(mBackendHandle->window, &rect)) {
            windowX = rect.left;
            windowY = rect.top;
        }
        RECT* tileClientArea = 0;
        GetClientRect(mBackendHandle->window, tileClientArea);

        u32 width = tileClientArea->right - tileClientArea->left;

        SetWindowPos(mBackendHandle->window, mBackendHandle->window, windowX, windowY, width, height, SWP_NOREPOSITION);

        mProps.height = height;
    }

    void windowsWindow::setTitle(const char* title) {
        SetWindowTextA(mBackendHandle->window, title);
    }

    void windowsWindow::setVsync(bool vsync) {
        CALMAR_WARN("Cannot set Vsync for Windows-API Window.");
    }

    void windowsWindow::setResizable(bool resizable) {
        if (!mBackendHandle->window) {
            mWindowStyle |= WS_THICKFRAME;
            mProps.resizable = resizable;
        } else
            CALMAR_WARN("Cannot set Windows-API window resizable flag after window creation.");
    }

    void windowsWindow::setPosition(u32 xpos, u32 ypos) {
        RECT* tileClientArea = 0;
        GetClientRect(mBackendHandle->window, tileClientArea);

        u32 width = tileClientArea->right - tileClientArea->left;
        u32 height = tileClientArea->bottom - tileClientArea->top;

        SetWindowPos(mBackendHandle->window, mBackendHandle->window, xpos, ypos, width, height, SWP_NOREPOSITION);

        mProps.height = height;
    }

    void* windowsWindow::getBackendHandle() const {
        return mBackendHandle;
    }

    bool windowsWindow::closeRequested() const {
        return !updateMessages();
    }

    double windowsWindow::getAbsoluteTime() const {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return (double)currentTime.QuadPart * mClockFrequency;
    }
    void windowsWindow::initBackend() {
        switch (mProps.renderBackend) {
            case renderingBackend::OPENGL:
                wglLoad::init();
                CALMAR_INFO("Initializing Win-API window with OpenGL rendering backend.");
                break;
            case renderingBackend::DIRECT3D:
                CALMAR_WARN("Direct3D is currently not supported as a Win-API rendering backend. Intializing Win-API window without rendering backend.");
                break;
            case renderingBackend::VULKAN:
                CALMAR_WARN("Vulkan is currently not supported as a Win-API rendering backend. Intializing Win-API window without rendering backend.");
                break;
            default:
                CALMAR_WARN("Initializing Win-API window without rendering backend.");
        }
        mBackendHandle = new windowsHandle();

        mBackendHandle->instance = GetModuleHandleA(0);

        HICON icon = LoadIcon(mBackendHandle->instance, IDI_APPLICATION);
        WNDCLASSA windowClass;
        windowClass.style = CBN_DBLCLK;
        windowClass.lpfnWndProc = updateMessagesWindowsWindow;
        windowClass.cbWndExtra = 0;
        windowClass.cbClsExtra = 0;
        windowClass.hInstance = mBackendHandle->instance;
        windowClass.hIcon = icon;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hbrBackground = NULL;
        windowClass.lpszClassName = "calmarWindowsWindow";

        CALMAR_ASSERT_MSG(RegisterClassA(&windowClass), "Failed to register windows window.");

        u32 clientX = mProps.xpos;
        u32 clientY = mProps.ypos;
        u32 clientWidth = mProps.width;
        u32 clientHeight = mProps.height;

        u32 windowX = clientX;
        u32 windowY = clientY;
        u32 windowWidth = clientWidth;
        u32 windowHeight = clientHeight;

        mWindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        u32 windowExStyle = WS_EX_APPWINDOW;

        mWindowStyle |= WS_MINIMIZEBOX;
        if (mProps.resizable) {
            mWindowStyle |= WS_THICKFRAME;
            mWindowStyle |= WS_MAXIMIZEBOX;
        }

        RECT borderRect = {0, 0, 0, 0};
        AdjustWindowRectEx(&borderRect, mWindowStyle, 0, windowExStyle);

        windowX += borderRect.left;
        windowY += borderRect.top;

        windowWidth += borderRect.right - borderRect.left;
        windowWidth += borderRect.bottom + borderRect.top;

        HWND handle = CreateWindowExA(windowExStyle, "calmarWindowsWindow",
                                      mProps.title, mWindowStyle, windowX, windowY, windowWidth, windowHeight,
                                      0, 0, mBackendHandle->instance, 0);

        if (mProps.fullscreen)
            CALMAR_WARN("Full-screened Win-API windows are currently not supported.");
        CALMAR_ASSERT_MSG(handle, "Failed to create windows window.");

        mBackendHandle->window = handle;

        SetWindowLongPtr(mBackendHandle->window, GWLP_USERDATA, (LONG_PTR)this);

        bool activate = true;
        i32 showWindowFlags = activate ? SW_SHOW : SW_SHOWNOACTIVATE;

        ShowWindow(mBackendHandle->window, showWindowFlags);

        CALMAR_INFO("Created Windows-API Window: '{0}' ({1}x{2}), Position on screen: {3}x{4}\t Vsync: {5} | Resizable: {6} | Fullscreen {7}",
                    mProps.title,
                    mProps.width,
                    mProps.height,
                    mProps.xpos,
                    mProps.ypos,
                    mProps.vsync ? "On" : "Off",
                    mProps.resizable ? "On" : "Off",
                    mProps.fullscreen ? "On" : "Off");

        HDC deviceContext = GetDC(mBackendHandle->window);

        int pixelFormatAttributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0};

        int pixelFormat = 0;
        UINT numFormats = 0;
        bool res = wglChoosePixelFormatARB(deviceContext, pixelFormatAttributes, nullptr, 1, &pixelFormat, &numFormats);

        CALMAR_ASSERT_MSG(res, "Failed to choose pixel format.");

        PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
        DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
        SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc);

        int opengGlAttributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0};

        mRenderContext = wglCreateContextAttribsARB(deviceContext, 0, opengGlAttributes);
        CALMAR_ASSERT_MSG(mRenderContext, "Failed to create rendering context.");

        mDeviceContext = deviceContext;

        wglMakeCurrent(mDeviceContext, mRenderContext);

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        mClockFrequency = 1.0 / (double)frequency.QuadPart;
        QueryPerformanceCounter(&mStartTime);
    }
    void windowsWindow::shutdownBackend() {
        DestroyWindow(mBackendHandle->window);
        wglDeleteContext(mRenderContext);
        delete mBackendHandle;
    }

    LRESULT CALLBACK updateMessagesWindowsWindow(HWND window, u32 msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_ERASEBKGND:
                return 1;
            case WM_CLOSE:
                application::getInstance()->evDispatcher.dispatch(windowCloseEvent());
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_SIZE: {
                // RECT r;
                // GetClientRect(window, &r);
                // u32 width = r.right - r.left;
                // u32 height = r.bottom - r.top;

                // TODO: Dispatch event
            } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);

                u32 key = (u32)wParam;

                windowsInput::processKey(key, pressed);

            } break;
            case WM_MOUSEMOVE: {
                u32 xpos = GET_X_LPARAM(lParam);
                u32 ypos = GET_Y_LPARAM(lParam);

                windowsInput::processMouseMove(xpos, ypos);
            } break;
            case WM_MOUSEWHEEL: {
                u32 yDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (yDelta != 0) {
                    yDelta = (yDelta < 0) ? -1 : 1;

                    windowsInput::processMouseWheel(yDelta);
                }
            } break;
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP: {
                bool pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
                u32 button = button::windows::Max;
                switch (msg) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        button = button::windows::Left;
                        break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        button = button::windows::Middle;
                        break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        button = button::windows::Right;
                        break;
                    default:
                        break;
                }

                if (button != button::windows::Max)
                    windowsInput::processButton(button, pressed);
            } break;
        }

        return DefWindowProcA(window, msg, wParam, lParam);
    }

    bool windowsWindow::updateMessages() const {
        MSG msg;
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

}  // namespace calmar
