#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/core/window.hpp"

namespace calmar {

    class input {
       public:
        static void init(const windowingBackend& backend);

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

        static void update();

       private:
        static windowingBackend mBackend;
    };
}  // namespace calmar
