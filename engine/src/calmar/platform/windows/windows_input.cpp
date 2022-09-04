#include "windows_input.hpp"

#include "calmar/core/application.hpp"

#include "calmar/event_system/key_events.hpp"
#include "calmar/event_system/mouse_events.hpp"

#include <memory.h>

namespace calmar {
    windowsInputStruct windowsInput::mInput;

    bool windowsInput::keyWentDown(u32 key) {
        return keyChanged(key) && mInput.keyboardNow.keys[key];
    }

    bool windowsInput::isKeyDown(u32 key) {
        return mInput.keyboardNow.keys[key];
    }

    bool windowsInput::isKeyReleased(u32 key) {
        return keyChanged(key) && !mInput.keyboardNow.keys[key];
    }

    bool windowsInput::keyChanged(u32 key) {
        return mInput.keyboardPrev.keys[key] != mInput.keyboardNow.keys[key];
    }

    bool windowsInput::mouseButtonWentDown(u32 button) {
        return mouseButtonChanged(button) && mInput.mouseNow.buttons[button];
    }

    bool windowsInput::isMouseButtonDown(u32 button) {
        return mInput.mouseNow.buttons[button];
    }

    bool windowsInput::isMouseButtonReleased(u32 button) {
        return mouseButtonChanged(button) && !mInput.mouseNow.buttons[button];
    }

    bool windowsInput::mouseButtonChanged(u32 button) {
        return mInput.mousePrev.buttons[button] != mInput.mouseNow.buttons[button];
    }

    i32 windowsInput::getMouseX() {
        return mInput.mouseNow.xpos;
    }

    i32 windowsInput::getMouseY() {
        return mInput.mouseNow.ypos;
    }

    i32 windowsInput::getMouseScrollX() {
        return mInput.mouseNow.scrollX;
    }

    i32 windowsInput::getMouseScrollY() {
        return mInput.mouseNow.scrollY;
    }

    i32 windowsInput::getMouseXDelta() {
        return mInput.mouseNow.xpos - mInput.mousePrev.xpos;
    }

    i32 windowsInput::getMouseYDelta() {
        return mInput.mouseNow.ypos - mInput.mousePrev.ypos;
    }

    void windowsInput::processKey(u32 key, bool pressed) {
        if (mInput.keyboardNow.keys[key] != pressed) {
            mInput.keyboardNow.keys[key] = pressed;

            if (pressed)
                application::getInstance()->evDispatcher.dispatch(keyPressedEvent(key));
            else
                application::getInstance()->evDispatcher.dispatch(keyReleasedEvent(key));
        }
    }

    void windowsInput::processButton(u32 button, bool pressed) {
        if (mInput.mouseNow.buttons[button] != pressed) {
            mInput.mouseNow.buttons[button] = pressed;

            if (pressed)
                application::getInstance()->evDispatcher.dispatch(mousePressedEvent(button));
            else
                application::getInstance()->evDispatcher.dispatch(mouseReleasedEvent(button));
        }
    }

    void windowsInput::processMouseMove(u32 x, u32 y) {
        if (mInput.mouseNow.xpos != x || mInput.mouseNow.ypos != y) {
            mInput.mouseNow.xpos = x;
            mInput.mouseNow.ypos = y;

            application::getInstance()->evDispatcher.dispatch(mouseMovedEvent(x, y));
        }
    }

    void windowsInput::processMouseWheel(u32 yDelta) {
        mInput.mouseNow.scrollX = 0;
        mInput.mouseNow.scrollY = yDelta;

        application::getInstance()->evDispatcher.dispatch(mouseScrolledEvent(0, yDelta));
    }

    void windowsInput::update() {
        mInput.keyboardPrev = mInput.keyboardNow;
        mInput.mousePrev = mInput.mouseNow;
    }

}  // namespace calmar
