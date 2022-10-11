#pragma once

#include "system.hpp"

namespace calmar {
    class scene : public systemEcs {
       public:
        scene();

        ~scene();

        void init();

        void update();

       private:
    };
}  // namespace calmar
