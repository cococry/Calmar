#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/input/input.hpp"

#include "calmar/renderer/render_command.hpp"

calmar::application* calmar::application::mInstance = nullptr;

namespace calmar {
    application::application(const windowProperties& windowProps) {
        logging::init();

        CALMAR_ASSERT_MSG(mInstance == nullptr, "Tried to instantiate application more than once.");

        mInstance = this;
        mRunning = true;

        mWindow = window::createScoped(windowProps);

        evDispatcher.listen(windowCloseEvent::evType, EVENT_CALLBACK(application::handleEvents));

        evDispatcher.listen(windowResizeEvent::evType, EVENT_CALLBACK(application::handleEvents));

        input::init(windowProps.backened);

        appRenderer.initSubsystems(windowProps.renderBackend);
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
            if (mWindow->closeRequested()) {
                close();
            }

            renderCommand::clearBuffers(clearBuffers::colorBuffer);
            renderCommand::clearColor({0.2f, 0.3f, 0.8f, 1.0f});

            mWindow->update();

            input::update();
        }
    }

    void application::handleEvents(const event& ev) {
        if (COMPARE_EVENTS(ev, windowCloseEvent)) {
            mRunning = false;
        }
        if (COMPARE_EVENTS(ev, windowResizeEvent)) {
            const windowResizeEvent& resizeEvent = static_cast<const windowResizeEvent&>(ev);
            glViewport(0, 0, resizeEvent.getWidth(), resizeEvent.getHeight());
        }
    }

}  // namespace calmar