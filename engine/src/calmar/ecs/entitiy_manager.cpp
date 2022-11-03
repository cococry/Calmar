#include "pch.h"
#include "entity_manager.hpp"

namespace calmar {
    entityManager::entityManager() {
        for (entity entty = 0; entty < MAX_ENTITIES; entty++) {
            mAvailableEntities.push(entty);
        }
    }

    entity entityManager::createEntity() {
        CALMAR_ASSERT_MSG(mEntityCount < MAX_ENTITIES, "Cannot create more than {0} entities in scene", MAX_ENTITIES);

        entity id = mAvailableEntities.front();
        mAvailableEntities.pop();
        mEntityCount++;

        return id;
    }

    void entityManager::destroyEntity(entity entty) {
        CALMAR_ASSERT_MSG(entty < MAX_ENTITIES, "Tried to delete entity out of entity range.");

        mComponentSets[entty].reset();

        mAvailableEntities.push(entty);
        mEntityCount--;
    }

    void entityManager::setComponentSet(entity entty, componentSet set) {
        CALMAR_ASSERT_MSG(entty < MAX_ENTITIES, "Tried to set component set of entity out of entity range.");

        mComponentSets[entty] = set;
    }

    componentSet entityManager::getComponentSet(entity entty) {
        CALMAR_ASSERT_MSG(entty < MAX_ENTITIES, "Tried to get component set of entity out of entity range.");

        return mComponentSets[entty];
    }
}  // namespace calmar
