#include "windows_util.hpp"

#include "calmar/core/application.hpp"

#include "ext/commdlg_clang.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace calmar {
    namespace platform {
        std::string windowsFileDialogs::openFile(const char* filter, const char* dir) {
            OPENFILENAMEA ofn;
            CHAR szFile[260] = {0};
            ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
            ofn.lStructSize = sizeof(OPENFILENAMEA);
            ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)application::getInstance()->display->getBackendHandle());
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = filter;
            ofn.lpstrInitialDir = dir;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
            if (GetOpenFileNameA(&ofn) == TRUE) {
                return ofn.lpstrFile;
            }
            return std::string();
        }
    }  // namespace platform
}  // namespace calmar
