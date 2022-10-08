#include "ecs.hpp"

namespace calmar {
    void ecs::init() {
        mComponentManager = std::make_unique<componentManager>();
        mEntityManager = std::make_unique<entityManager>();
        mSystemManager = std::make_unique<systemManager>();
    }
    entity ecs::createEntity() {
        return mEntityManager->createEntity();
    }
    void ecs::destroyEntity(entity entty) {
        mEntityManager->destroyEntity(entty);

        mComponentManager->entityDestroyed(entty);

        mSystemManager->entityDestroyed(entty);
    }
}  // namespace calmar
