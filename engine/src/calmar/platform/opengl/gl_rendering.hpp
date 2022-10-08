#pragma once

#include "calmar/core/defines.hpp"

namespace calmar {
    class glRendering {
       public:
        static void init();

        static const u8* getVersion();
    };
}  // namespace calmar
