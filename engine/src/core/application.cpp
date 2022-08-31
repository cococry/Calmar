#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

calmar::application* calmar::application::mInstance = nullptr;

namespace calmar {
    application::application(const windowProperties& props) {
        logging::init();

        CALMAR_ASSERT_MSG(mInstance == nullptr, "Tried to instantiate application more than once.");

        mInstance = this;
        mRunning = true;

        mWindow = window::createScoped(props);
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
            if (mWindow->closeRequested())
                close();
            mWindow->update();
        }
    }

}  // namespace calmar
