#include "pch.h"
#include "ecs.hpp"

#include "components.hpp"

namespace calmar {
    void ecs::init() {
        mComponentManager = std::make_unique<componentManager>();
        mEntityManager = std::make_unique<entityManager>();
        mSystemManager = std::make_unique<systemManager>();
    }
    entity ecs::createEntity() {
        entity ret = mEntityManager->createEntity();
        addComponent(ret, transformComponent());
        addComponent(ret, tagComponent("Entity"));
        return ret;
    }
    void ecs::destroyEntity(entity entty) {
        mEntityManager->destroyEntity(entty);

        mComponentManager->entityDestroyed(entty);

        mSystemManager->entityDestroyed(entty);
    }
}  // namespace calmar
