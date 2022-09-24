#pragma once

#include "logging.hpp"

// Checks if a given condition is met and if it is not,
// it breaks the debugger.
#define CALMAR_ASSERT(cond)                                                                                   \
    {                                                                                                         \
        if (cond) {                                                                                           \
        } else {                                                                                              \
            CALMAR_CRITICAL("Assertion Failed: '{0}' in file '{1}' on line {2}.", #cond, __FILE__, __LINE__); \
            __debugbreak();                                                                                   \
        }                                                                                                     \
    }

// Checks if a given condition is met and if t is not,
// it breaks the debugger and prints out a given message
#define CALMAR_ASSERT_MSG(cond, ...)                                                                                             \
    {                                                                                                                            \
        if (cond) {                                                                                                              \
        } else {                                                                                                                 \
            CALMAR_CRITICAL("Assertion Failed: '{0}' in file '{1}' on line {2}. '{3}'", #cond, __FILE__, __LINE__, __VA_ARGS__); \
            __debugbreak();                                                                                                      \
        }                                                                                                                        \
    }
