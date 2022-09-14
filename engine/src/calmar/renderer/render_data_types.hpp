#pragma once

#include "calmar/core/defines.hpp"

namespace calmar {
    /* This struct is used to abstract data types from renderer APIs*/
    struct renderDataTypes {
        /// @brief Initializes the static member variables
        static void init();

        static u32 byte8;
        static u32 unsignedByte8;
        static u32 short16;
        static u32 unsignedShort16;
        static u32 int32;
        static u32 unsignedInt32;
        static u32 float32;
    };
}  // namespace calmar
