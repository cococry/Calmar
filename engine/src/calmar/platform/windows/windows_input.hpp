#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/input/mouse_codes.hpp"

#include "calmar/core/window.hpp"

namespace calmar {
    struct windowsKeyboardStruct {
        bool keys[256];
    };

    struct windowsMouseStruct {
        bool buttons[button::windows::Max];

        i32 ypos = 0, xpos = 0;

        i32 scrollX = 0, scrollY = 0;
    };

    struct windowsInputStruct {
        windowsKeyboardStruct keyboardPrev;
        windowsKeyboardStruct keyboardNow;
        windowsMouseStruct mousePrev;
        windowsMouseStruct mouseNow;
    };

    class windowsInput {
       public:
        static bool keyWentDown(u32 key);

        static bool isKeyDown(u32 key);

        static bool isKeyReleased(u32 key);

        static bool keyChanged(u32 key);

        static bool mouseButtonWentDown(u32 button);

        static bool isMouseButtonDown(u32 button);

        static bool isMouseButtonReleased(u32 button);

        static bool mouseButtonChanged(u32 button);

        static i32 getMouseX();

        static i32 getMouseY();

        static i32 getMouseScrollX();

        static i32 getMouseScrollY();

        static i32 getMouseXDelta();

        static i32 getMouseYDelta();

        static void processKey(u32 key, bool pressed);

        static void processButton(u32 button, bool pressed);

        static void processMouseMove(u32 x, u32 y);

        static void processMouseWheel(u32 yDelta);

        static void update();

       private:
        static windowsInputStruct mInput;
    };
}  // namespace calmar
