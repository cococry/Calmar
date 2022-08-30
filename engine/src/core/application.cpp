#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

calmar::application* calmar::application::mInstance = nullptr;

namespace calmar {
    application::application() {
        logging::init();

        CALMAR_ASSERT_MSG(mInstance == nullptr, "Tried to instantiate application more than once.");

        mInstance = this;
        mRunning = true;

        CALMAR_DEBUG("Running application");
    }

    application::~application() {
        close();
    }

    void application::run() {
        while (mRunning) {
        }
    }

}  // namespace calmar
