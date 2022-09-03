#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "global_state.hpp"

#include "calmar/event_system/window_events.hpp"

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
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
            if (mWindow->closeRequested()) {
                evDispatcher.dispatch(windowCloseEvent());
                close();
            }

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

            mWindow->update();
        }
    }

    void application::handleEvents(const event& ev) {
        if (COMPARE_EVENTS(ev, windowCloseEvent)) {
            mRunning = false;
        }
    }

}  // namespace calmar