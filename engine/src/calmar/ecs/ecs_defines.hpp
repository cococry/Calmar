#pragma once

#include "calmar/core/defines.hpp"

#include <bitset>

namespace calmar {
    using entity = i32;

    const entity MAX_ENTITIES = 16000;

    using componentType = u8;

    const componentType MAX_COMPONENTS_ON_ENTITY = 32;

    using componentSet = std::bitset<MAX_COMPONENTS_ON_ENTITY>;
}  // namespace calmar
