#pragma once

#include "ecs_defines.hpp"

#include <queue>
#include <array>

namespace calmar {
    class entityManager {
       public:
        entityManager();

        entity createEntity();

        void destroyEntity(entity entty);

        void setComponentSet(entity entty, componentSet set);

        componentSet getComponentSet(entity entty);

       private:
        std::queue<entity> mAvailableEntities{};
        std::array<componentSet, MAX_ENTITIES> mComponentSets{};

        u32 mEntityCount = 0;
    };
}  // namespace calmar
