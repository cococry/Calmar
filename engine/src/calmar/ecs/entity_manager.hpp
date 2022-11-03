#pragma once

#include "ecs_defines.hpp"

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

        i32 mEntityCount = 0;
    };
}  // namespace calmar
