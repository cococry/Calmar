#pragma once

#include "calmar/core/defines.hpp"

namespace calmar {
    class input {
       public:
        static bool keyWentDown(u32 key);

        static bool isKeyDown(u32 key);

        static bool isKeyReleased(u32 key);

        static bool keyChanged(u32 key);

        static bool mouseButtonWentDown(u32 button);

        static bool isMouseButtonDown(u32 button);

        static bool isMouseButtonReleased(u32 button);

        static bool mouseButtonChanged(u32 button);

        static u32 getMouseX();

        static u32 getMouseY();

        static u32 getMouseScrollX();

        static u32 getMouseScrollY();

        static u32 getMouseXDelta();

        static u32 getMouseYDelta();

       private:
    };
}  // namespace calmar
