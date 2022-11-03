#include "pch.h"
#include "window.hpp"

#include "calmar/platform/glfw/window_glfw.hpp"

namespace calmar {
    window window::create(const windowProperties& props) {
        /* Checking the windowing backend */
        switch (props.backened) {
            // Instantiating and returning a glfwWindow for the GLFW backend
            case windowingBackend::GLFW:
                return glfwWindow(props);
                break;
            default:
                // Returning a undefined window if no vaild windowing backend is used
                return windowUndefined();
                break;
        }
        // Returning a undefined window if no vaild windowing backend is used
        return windowUndefined();
    }

    std::shared_ptr<window> window::createRef(const windowProperties& props) {
        switch (props.backened) {
            case windowingBackend::GLFW:
                return std::make_shared<glfwWindow>(props);
                break;
            default:
                return std::make_shared<windowUndefined>();
                break;
        }
        return std::make_shared<windowUndefined>();
    }

    std::unique_ptr<window> window::createScoped(const windowProperties& props) {
        switch (props.backened) {
            case windowingBackend::GLFW:
                return std::make_unique<glfwWindow>(props);
                break;
            default:
                return std::make_unique<windowUndefined>();
                break;
        }
        return std::make_unique<windowUndefined>();
    }

    const windowProperties& window::getProperties() const {
        return mProps;
    }

}  // namespace calmar
