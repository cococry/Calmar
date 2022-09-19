#include "glfw_input.hpp"

#include "calmar/event_system/key_events.hpp"
#include "calmar/event_system/mouse_events.hpp"
#include "calmar/core/application.hpp"

namespace calmar {
    glfwMouseStruct glfwInput::mMouse;
    glfwKeyboardStruct glfwInput::mKeyboard;

    bool glfwInput::keyWentDown(u32 key) {
        return keyChanged(key) && mKeyboard.keys[key];
    }

    bool glfwInput::isKeyDown(u32 key) {
        return mKeyboard.keys[key];
    }

    bool glfwInput::isKeyReleased(u32 key) {
        return keyChanged(key) && !mKeyboard.keys[key];
    }

    bool glfwInput::keyChanged(u32 key) {
        bool ret = mKeyboard.keysChanged[key];
        mKeyboard.keysChanged[key] = false;
        return ret;
    }

    bool glfwInput::mouseButtonWentDown(u32 button) {
        return mouseButtonChanged(button) && mMouse.activeButtons[button];
    }

    bool glfwInput::isMouseButtonDown(u32 button) {
        return mMouse.activeButtons[button];
    }

    bool glfwInput::isMouseButtonReleased(u32 button) {
        return mouseButtonChanged(button) && !mMouse.activeButtons[button];
    }

    bool glfwInput::mouseButtonChanged(u32 button) {
        bool ret = mMouse.changedButtons[button];
        mMouse.changedButtons[button] = false;
        return ret;
    }

    i32 glfwInput::getMouseX() {
        return mMouse.xpos;
    }

    i32 glfwInput::getMouseY() {
        return mMouse.ypos;
    }

    i32 glfwInput::getMouseScrollX() {
        return mMouse.scrollDeltaX;
    }

    i32 glfwInput::getMouseScrollY() {
        return mMouse.scrollDeltaY;
    }

    i32 glfwInput::getMouseXDelta() {
        return mMouse.xposDelta;
    }

    i32 glfwInput::getMouseYDelta() {
        return mMouse.xposDelta;
    }

    void glfwInput::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action != GLFW_RELEASE) {
            if (!mKeyboard.keys[key]) {
                mKeyboard.keys[key] = true;
            }
        } else {
            mKeyboard.keys[key] = false;
        }

        mKeyboard.keysChanged[key] = (action != GLFW_REPEAT);

        if (keyWentDown(key)) {
            application::getInstance()->evDispatcher.dispatch(keyPressedEvent(key));
        }
        mKeyboard.keysChanged[key] = (action != GLFW_REPEAT);

        if (isKeyReleased(key)) {
            application::getInstance()->evDispatcher.dispatch(keyReleasedEvent(key));
        }
        mKeyboard.keysChanged[key] = (action != GLFW_REPEAT);
    }

    void glfwInput::glfwButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action != GLFW_RELEASE) {
            if (!mMouse.activeButtons[button]) {
                mMouse.activeButtons[button] = true;
            }
        } else {
            mMouse.activeButtons[button] = false;
        }

        mMouse.changedButtons[button] = (action != GLFW_REPEAT);
        if (isMouseButtonDown(button)) {
            application::getInstance()->evDispatcher.dispatch(mousePressedEvent(button));
        }
        mMouse.changedButtons[button] = (action != GLFW_REPEAT);
        if (isMouseButtonReleased(button)) {
            application::getInstance()->evDispatcher.dispatch(mouseReleasedEvent(button));
        }
        mMouse.changedButtons[button] = (action != GLFW_REPEAT);
    }

    void glfwInput::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        mMouse.scrollDeltaX = xoffset;
        mMouse.scrollDeltaY = yoffset;

        application::getInstance()->evDispatcher.dispatch(mouseScrolledEvent(xoffset, yoffset));
    }

    void glfwInput::glfwCursorCallback(GLFWwindow* window, double xpos, double ypos) {
        mMouse.xpos = xpos;
        mMouse.ypos = ypos;

        if (mMouse.firstMouse) {
            mMouse.lastXpos = xpos;
            mMouse.lastYpos = ypos;
            mMouse.firstMouse = false;
        }

        mMouse.xposDelta = mMouse.xpos - mMouse.lastXpos;
        mMouse.yposDelta = mMouse.ypos - mMouse.lastYpos;
        mMouse.lastXpos = xpos;
        mMouse.lastYpos = ypos;

        application::getInstance()->evDispatcher.dispatch(mouseMovedEvent(xpos, ypos));
    }

    void glfwInput::update() {
    }

}  // namespace calmar
