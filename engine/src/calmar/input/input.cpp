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
        return false;
    }

    bool input::isKeyDown(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isKeyDown(key);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::isKeyReleased(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isKeyReleased(key);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::keyChanged(u32 key) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::keyChanged(key);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::mouseButtonWentDown(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::mouseButtonWentDown(button);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::isMouseButtonDown(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isMouseButtonDown(button);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::isMouseButtonReleased(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::isMouseButtonReleased(button);
                break;
            default:
                break;
        }
        return false;
    }

    bool input::mouseButtonChanged(u32 button) {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::mouseButtonChanged(button);
                break;
            default:
                break;
        }
        return false;
    }

    double input::getMouseX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseX();
                break;
            default:
                break;
        }
        return 0;
    }

    double input::getMouseY() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseY();
                break;
            default:
                break;
        }
        return 0;
    }

    double input::getMouseScrollX() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseScrollX();
                break;
            default:
                break;
        }
        return 0;
    }

    double input::getMouseScrollY() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseScrollY();
                break;
            default:
                break;
        }
        return 0;
    }

    double input::getMouseXDelta() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseXDelta();
                break;
            default:
                break;
        }
        return 0;
    }

    double input::getMouseYDelta() {
        switch (mBackend) {
            case windowingBackend::GLFW:
                return glfwInput::getMouseYDelta();
                break;
            default:
                break;
        }
        return 0;
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
