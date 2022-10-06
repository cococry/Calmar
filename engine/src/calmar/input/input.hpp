#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/core/window.hpp"

#include "calmar/core/application.hpp"

#define API_CODE(glfwCode, winApiCode) calmar::application::getInstance()->display->getProperties().backened == calmar::windowingBackend::GLFW ? glfwCode : winApiCode

namespace calmar {

    class input {
       public:
        /// @brief Initializes the input system by setting a windowing
        /// system to handle input with.
        /// @param backend The windowing backend to use for input handeling
        static void init(const windowingBackend& backend);

        /// @brief Returns if a given key went down is the previous frame
        /// @param key The keycode of the key to check the state of
        static bool keyWentDown(u32 key);

        /// @brief Returns if a given key is pressed in the current frame
        /// @param key The keycode of the key to check the state of
        static bool isKeyDown(u32 key);

        /// @brief Returns if a given key was released in the previous frame
        /// @param key The keycode of the key to check the state of
        static bool isKeyReleased(u32 key);

        /// @brief Returns if a given key has changed it's state from the previous frame
        /// to the current frame
        /// @param key The keycode of the key to check the state of
        static bool keyChanged(u32 key);

        /// @brief Returns if a given mouse button went down in the previous frame
        /// @param button The button code of the button to check the state of
        static bool mouseButtonWentDown(u32 button);

        /// @brief Returns if a given mouse button is pressed in the current frame
        /// @param button The button code of the mouse button to check the state of
        static bool isMouseButtonDown(u32 button);

        /// @brief Returns if a given mouse button was released in the previous frame
        /// @param button The button code of the mouse button to check the state of
        static bool isMouseButtonReleased(u32 button);

        /// @brief Returns if a given mose button has changed it's state from the previous frame
        /// to the current frame
        /// @param button The button code of the button to check the state of
        static bool mouseButtonChanged(u32 button);

        /// @brief Returns the x position of the mouse in the current frame
        static i32 getMouseX();

        /// @brief Returns the y position of the mouse in the current frame
        static i32 getMouseY();

        /// @brief Returns the x scoll amout of the mouse in the current frame
        static i32 getMouseScrollX();

        /// @brief Returns the y scoll amout of the mouse in the current frame
        static i32 getMouseScrollY();

        /// @brief Returns the x scoll amout delta from the last frame to
        /// the current frame  of the mouse in the current frame
        static i32 getMouseXDelta();

        /// @brief Returns the y scoll amout delta from the last frame to
        /// the current frame  of the mouse in the current frame
        static i32 getMouseYDelta();

        /// @brief Updates the input system for the implmented windowing api if it is needed
        static void update();

       private:
        static windowingBackend mBackend;
    };
}  // namespace calmar
