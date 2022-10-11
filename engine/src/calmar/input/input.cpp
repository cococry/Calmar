#include "input.hpp"

#include "calmar/platform/glfw/glfw_input.hpp"

#include "calmar/core/logging.hpp"

#include "key_codes.hpp"

namespace calmar {
    windowingBackend input::mBackend;
    void input::init(const windowingBackend& backend) {
        // Setting the windowing backend or input
        mBackend = backend;
    }

    bool input::keyWentDown(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::keyWentDown(key);
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
            default:
                break;
        }
    }

    bool input::isKeyReleased(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isKeyReleased(key);
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
            default:
                break;
        }
    }

    bool input::mouseButtonWentDown(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::mouseButtonWentDown(button);
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
            default:
                break;
        }
    }

    bool input::isMouseButtonReleased(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isMouseButtonReleased(button);
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
            default:
                break;
        }
    }

    i32 input::getMouseX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseX();
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
            default:
                break;
        }
    }

    i32 input::getMouseScrollX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseScrollX();
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
            default:
                break;
        }
    }

    i32 input::getMouseXDelta() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseXDelta();
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
            default:
                break;
        }
    }

    void input::update() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                glfwInput::update();
                break;
            default:
                break;
        }
    }

}  // namespace calmar
