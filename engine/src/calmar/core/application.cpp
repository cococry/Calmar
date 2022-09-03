#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "global_state.hpp"

#include <gl/GL.h>

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
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
            if (mWindow->closeRequested())
                close();

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

            mWindow->update();
        }
    }
}  // namespace calmar