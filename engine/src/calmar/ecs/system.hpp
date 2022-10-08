#pragma once

#include "ecs_defines.hpp"

#include <set>

namespace calmar {
    class CALMAR_API systemEcs {
       public:
        std::set<entity> mEntities;
    };

}  // namespace calmar
