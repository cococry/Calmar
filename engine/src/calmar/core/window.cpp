#include "window.hpp"

#include "calmar/platform/window_glfw.hpp"
#include "calmar/platform/window_windows.hpp"
namespace calmar {
    window window::create(const windowProperties& props) {
        switch (props.backened) {
            case windowingBackend::GLFW:
                return glfwWindow(props);
                break;
            case windowingBackend::WINDOWS:
                return windowsWindow(props);
            default:
                return windowUndefined();
                break;
        }
        return windowUndefined();
    }

    std::shared_ptr<window> window::createRef(const windowProperties& props) {
        switch (props.backened) {
            case windowingBackend::GLFW:
                return std::make_shared<glfwWindow>(props);
                break;
            case windowingBackend::WINDOWS:
                return std::make_shared<windowsWindow>(props);
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
            case windowingBackend::WINDOWS:
                return std::make_unique<windowsWindow>(props);
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
