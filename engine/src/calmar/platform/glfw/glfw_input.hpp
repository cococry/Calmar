#pragma once

#include "calmar/core/defines.hpp"

#include <GLFW/glfw3.h>

namespace calmar {
    struct glfwKeyboardStruct {
        bool keys[GLFW_KEY_LAST];
        bool keysChanged[GLFW_KEY_LAST];
    };

    struct glfwMouseStruct {
        bool activeButtons[GLFW_KEY_LAST];
        bool changedButtons[GLFW_KEY_LAST];

        i32 ypos = 0, xpos = 0;
        i32 scrollDeltaX = 0, scrollDeltaY = 0;
        i32 lastXpos = 0, lastYpos = 0;
        i32 xposDelta = 0, yposDelta = 0;

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

        static i32 getMouseX();

        static i32 getMouseY();

        static i32 getMouseScrollX();

        static i32 getMouseScrollY();

        static i32 getMouseXDelta();

        static i32 getMouseYDelta();

        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void glfwButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void glfwCursorCallback(GLFWwindow* window, double xpos, double ypos);

        static void update();

       private:
        static glfwKeyboardStruct mKeyboard;
        static glfwMouseStruct mMouse;
    };
}  // namespace calmar
