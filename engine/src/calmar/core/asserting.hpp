#ifndef CALMAR_ASSERTING_INCLUDED
#define CALMAR_ASSERTING_INCLUDED

#include "logging.hpp"

#define CALMAR_ASSERT(cond)                                                                                   \
    {                                                                                                         \
        if (cond) {                                                                                           \
        } else {                                                                                              \
            CALMAR_CRITICAL("Assertion Failed: '{0}' in file '{1}' on line {2}.", #cond, __FILE__, __LINE__); \
        }                                                                                                     \
    }

#define CALMAR_ASSERT_MSG(cond, msg)                                                                                     \
    {                                                                                                                    \
        if (cond) {                                                                                                      \
        } else {                                                                                                         \
            CALMAR_CRITICAL("Assertion Failed: '{0}' in file '{1}' on line {2}. '{3}'", #cond, __FILE__, __LINE__, msg); \
        }                                                                                                                \
    }

#endif