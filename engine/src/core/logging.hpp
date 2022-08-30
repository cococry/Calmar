#ifndef CALMAR_LOGGING_INCLUDED
#define CALMAR_LOGGING_INCLUDED

#include "defines.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace calmar {
    class CALMAR_API logging {
        public:
            static void init();

            static inline const std::shared_ptr<spdlog::logger>& getLogger() {
                return mLogger;
            }

        private:
            static std::shared_ptr<spdlog::logger> mLogger;
    };
}  // namespace calmar

#define CALMAR_DEBUG(...) calmar::logging::getLogger()->debug(__VA_ARGS__)

#define CALMAR_TRACE(...) calmar::logging::getLogger()->trace(__VA_ARGS__)

#define CALMAR_INFO(...) calmar::logging::getLogger()->info(__VA_ARGS__)

#define CALMAR_WARN(...) calmar::logging::getLogger()->warn(__VA_ARGS__)

#define CALMAR_ERROR(...) calmar::logging::getLogger()->error(__VA_ARGS__)

#define CALMAR_CRITICAL(...) calmar::logging::getLogger()->critical(__VA_ARGS__)

#endif