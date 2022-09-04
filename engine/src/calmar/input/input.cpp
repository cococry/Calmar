#include "input.hpp"

#include "calmar/platform/glfw/glfw_input.hpp"
#include "calmar/platform/windows/windows_input.hpp"

#include "calmar/core/global_state.hpp"

#include "calmar/core/logging.hpp"

namespace calmar {
    windowingBackend input::mBackend;
    void input::init(const windowingBackend& backend) {
        mBackend = backend;
    }

    bool input::keyWentDown(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::keyWentDown(key);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::keyWentDown(key);
                break;
            default:
                break;
        }
    }

    bool input::isKeyDown(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isKeyDown(key);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::isKeyDown(key);
                break;
            default:
                break;
        }
    }

    bool input::isKeyReleased(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isKeyReleased(key);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::isKeyReleased(key);
                break;
            default:
                break;
        }
    }

    bool input::keyChanged(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::keyChanged(key);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::keyChanged(key);
                break;
            default:
                break;
        }
    }

    bool input::mouseButtonWentDown(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::mouseButtonWentDown(button);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::mouseButtonWentDown(button);
                break;
            default:
                break;
        }
    }

    bool input::isMouseButtonDown(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isMouseButtonDown(button);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::isMouseButtonDown(button);
                break;
            default:
                break;
        }
    }

    bool input::isMouseButtonReleased(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isMouseButtonReleased(button);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::isMouseButtonReleased(button);
                break;
            default:
                break;
        }
    }

    bool input::mouseButtonChanged(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::mouseButtonChanged(button);
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::mouseButtonChanged(button);
                break;
            default:
                break;
        }
    }

    i32 input::getMouseX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseX();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseX();
                break;
            default:
                break;
        }
    }

    i32 input::getMouseY() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseY();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseY();
                break;
            default:
                break;
        }
    }

    i32 input::getMouseScrollX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseScrollX();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseScrollX();
                break;
            default:
                break;
        }
    }

    i32 input::getMouseScrollY() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseScrollY();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseScrollY();
                break;
            default:
                break;
        }
    }

    i32 input::getMouseXDelta() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseXDelta();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseXDelta();
                break;
            default:
                break;
        }
    }

    i32 input::getMouseYDelta() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseYDelta();
                break;
            case windowingBackend::WINDOWS:
                return windowsInput::getMouseYDelta();
                break;
            default:
                break;
        }
    }

    void input::update() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                glfwInput::update();
                break;
            case windowingBackend::WINDOWS:
                windowsInput::update();
                break;
            default:
                break;
        }
    }

}  // namespace calmar
