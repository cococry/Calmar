#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "global_state.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/input/input.hpp"

#include "calmar/input/mouse_codes.hpp"

#include "calmar/input/key_codes.hpp"

#include <glad/glad.h>

calmar::application* calmar::application::mInstance = nullptr;

namespace calmar {
    application::application(const windowProperties& windowProps) {
        logging::init();

        CALMAR_ASSERT_MSG(mInstance == nullptr, "Tried to instantiate application more than once.");

        mInstance = this;
        mRunning = true;

        gState.renderBackend = windowProps.renderBackend;
        gState.windowBackend = windowProps.backened;

        mWindow = window::createScoped(windowProps);

        evDispatcher.listen(windowCloseEvent::evType, EVENT_CALLBACK(application::handleEvents));

        evDispatcher.listen(windowResizeEvent::evType, EVENT_CALLBACK(application::handleEvents));
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
            if (mWindow->closeRequested()) {
                close();
            }

            CALMAR_INFO("{0}, {1}", input::getMouseX(), input::getMouseY());

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

            mWindow->update();
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