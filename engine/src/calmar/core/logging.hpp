#ifndef CALMAR_LOGGING_INCLUDED
#define CALMAR_LOGGING_INCLUDED

#include "defines.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace calmar {
    /*  This static class handles logging in the engine. The logging backend which is
        used is spdlog. The static "init()" method has to be called before any functionality
        of the class is used in order to prevent undefined behaviour.*/
    class logging {
       public:
        /// @brief Initializes the logging subsystem of the engine by setting a spdlog logging pattern
        // and intializing the "logger" member variable
        static void init();

        /// @brief This returns the shared pointer instance of the spdlog logger object.
        /// It is used to use logging functionality from spdlog
        /// @return The member static variable "logger" of the class
        static inline const std::shared_ptr<spdlog::logger>& getLogger() {
            return mLogger;
        }

       private:
        /// @brief The shared pointer instance of the spdlog logger object
        static std::shared_ptr<spdlog::logger> mLogger;
    };
}  // namespace calmar

// A simple alias which uses the logging class to log a debug level message with spdlog
#define CALMAR_DEBUG(...) calmar::logging::getLogger()->debug(__VA_ARGS__)

// A simple alias which uses the logging class to log a trace level message with spdlog
#define CALMAR_TRACE(...) calmar::logging::getLogger()->trace(__VA_ARGS__)

// A simple alias which uses the logging class to log a info level message with spdlog
#define CALMAR_INFO(...) calmar::logging::getLogger()->info(__VA_ARGS__)

// A simple alias which uses the logging class to log a info warn message with spdlog
#define CALMAR_WARN(...) calmar::logging::getLogger()->warn(__VA_ARGS__)

// A simple alias which uses the logging class to log a info error message with spdlog
#define CALMAR_ERROR(...) calmar::logging::getLogger()->error(__VA_ARGS__)

// A simple alias which uses the logging class to log a crical level message with spdlog
#define CALMAR_CRITICAL(...) calmar::logging::getLogger()->critical(__VA_ARGS__)

#endif