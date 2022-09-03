#pragma once

#include "calmar/core/defines.hpp"

#include <GLFW/glfw3.h>

namespace calmar {
    struct keyboardStruct {
        bool keys[GLFW_KEY_LAST];
        bool keysChanged[GLFW_KEY_LAST];
    };

    struct mouseStruct {
        bool activeButtons[GLFW_KEY_LAST];
        bool changedButtons[GLFW_KEY_LAST];

        double ypos = 0, xpos = 0;
        double scrollDeltaX = 0, scrollDeltaY = 0;
        double lastXpos = 0, lastYpos = 0;
        double xposDelta = 0, yposDelta = 0;

        bool firstMouse = true;
    };
    class glfwInput {
       public:
        static bool keyWentDown(u32 key);

        static bool isKeyDown(u32 key);

        static bool isKeyReleased(u32 key);

        static bool keyChanged(u32 key);

        static bool mouseButtonWentDown(u32 button);

        static bool isMouseButtonDown(u32 button);

        static bool isMouseButtonReleased(u32 button);

        static bool mouseButtonChanged(u32 buton);

        static u32 getMouseX();

        static u32 getMouseY();

        static u32 getMouseScrollX();

        static u32 getMouseScrollY();

        static u32 getMouseXDelta();

        static u32 getMouseYDelta();

        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void glfwButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void glfwCursorCallback(GLFWwindow* window, double xpos, double ypos);

       private:
        static keyboardStruct mKeyboard;
        static mouseStruct mMouse;
    };
}  // namespace calmar
