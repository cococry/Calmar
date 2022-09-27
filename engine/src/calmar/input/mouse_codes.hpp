#pragma once

#include "calmar/core/defines.hpp"

using mouseCode = u32;

namespace calmar {
    namespace button {
        namespace windows {

            enum : mouseCode {
                Left = 0x01,
                Right = 0x02,
                Middle = 0x04,
                Max
            };
        }  // namespace windows

        namespace glfw {
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
        }  // namespace glfw
    }      // namespace button
}  // namespace calmar