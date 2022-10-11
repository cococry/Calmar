#pragma once

#include "calmar/core/defines.hpp"

using mouseCode = u32;

namespace calmar {
    namespace button {

        enum : mouseCode {
            b0 = 0,
            b1 = 1,
            b2 = 2,
            b3 = 3,
            b4 = 4,
            b5 = 5,
            b6 = 6,
            b7 = 7,

            Last = 7,
            Left = 0,
            Right = 1,
            Middle = 2
        };
    }  // namespace button
}  // namespace calmar