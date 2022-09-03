#include "wgl_load.hpp"

#include <Windows.h>
#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include "calmar/core/asserting.hpp"

namespace calmar {
    void wglLoad::init() {
        WNDCLASSEX windowClass = {};
        windowClass.style = CS_OWNDC;
        windowClass.lpfnWndProc = DefWindowProcA;
        windowClass.lpszClassName = TEXT("gladLoadingWindow");
        windowClass.cbSize = sizeof(WNDCLASSEX);

        auto classId = RegisterClassEx(&windowClass);

        HWND gladLoadingWindow = CreateWindowEx(
            0,
            MAKEINTATOM(classId),
            TEXT("OGL3DDummyWindow"),
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            windowClass.hInstance,
            0);

        CALMAR_ASSERT_MSG(gladLoadingWindow, "Failed to create Glad-loading window.");

        HDC gladLoadingDC = GetDC(gladLoadingWindow);

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pixelFormat = ChoosePixelFormat(gladLoadingDC, &pfd);
        SetPixelFormat(gladLoadingDC, pixelFormat, &pfd);

        HGLRC gladLoadingContext = wglCreateContext(gladLoadingDC);
        CALMAR_ASSERT_MSG(gladLoadingContext, "Failed to initialize Glad-loading rendering context.");

        bool res = wglMakeCurrent(gladLoadingDC, gladLoadingContext);
        CALMAR_ASSERT_MSG(res, "Failed to set Glad-loading rendering context.");

        CALMAR_ASSERT_MSG(gladLoadWGL(gladLoadingDC), "Failed to initialize Glad with WGL.");

        CALMAR_ASSERT_MSG(gladLoadGL(), "Failed to load Glad.");

        wglMakeCurrent(gladLoadingDC, 0);
        wglDeleteContext(gladLoadingContext);
        ReleaseDC(gladLoadingWindow, gladLoadingDC);
        DestroyWindow(gladLoadingWindow);
    }
}  // namespace calmar
