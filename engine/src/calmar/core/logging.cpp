#include "pch.h"
#include "logging.hpp"

std::shared_ptr<spdlog::logger> calmar::logging::mLogger = nullptr;

namespace calmar {
    void logging::init() {
        // Setting the spdlog pattern
        // Example message: [12:29:13] CALMAR: This is an example.
        spdlog::set_pattern("%^[%T] %n: %v%$");

        // Initializing the spdlog logger instance and naming it
        mLogger = spdlog::stdout_color_mt("CALMAR");
        mLogger->set_level(spdlog::level::trace);
    }
}  // namespace calmar
