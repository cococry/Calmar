#include "logging.hpp"

std::shared_ptr<spdlog::logger> calmar::logging::mLogger = nullptr;

namespace calmar {
    void logging::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        mLogger = spdlog::stdout_color_mt("CALMAR");
        mLogger->set_level(spdlog::level::trace);
    }
} // namespace calmar
