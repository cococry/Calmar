#pragma once

#include "ecs_defines.hpp"

#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

#include <memory>

namespace calmar {
    class ecs {
       public:
        void init();

        entity createEntity();

        void destroyEntity(entity entty);

        template <typename T>
        void registerComponent() {
            mComponentManager->registerComponent<T>();
        }

        template <typename T>
        void addComponent(entity entty, T component) {
            mComponentManager->addComponent<T>(entty, component);

            auto componentSet = mEntityManager->getComponentSet(entty);
            componentSet.set(mComponentManager->getComponentType<T>(), true);
            mEntityManager->setComponentSet(entty, componentSet);

            mSystemManager->entityComponentSetChanged(entty, componentSet);
        }

        template <typename T>
        void removeComponent(entity entty) {
            mComponentManager->removeComponent<T>(entty);

            auto componentSet = mEntityManager->getComponentSet(entty);
            componentSet.set(mComponentManager->getComponentType<T>(), false);
            mEntityManager->setComponentSet(entty, componentSet);

            mSystemManager->EntitySignatureChanged(entty, componentSet);
        }

        template <typename T>
        T& getComponent(entity entty) {
            return mComponentManager->getComponent<T>(entty);
        }

        template <typename T>
        bool hasComponent(entity entty) {
            return mComponentManager->hasComponent<T>(entty);
        }

        template <typename T>
        componentType getComponentType() {
            return mComponentManager->getComponentType<T>();
        }

        template <typename T>
        std::shared_ptr<T> registerSystem() {
            return mSystemManager->registerSystem<T>();
        }

        template <typename T>
        void setSystemComponentSet(componentSet set) {
            mSystemManager->setComponentSet<T>(set);
        }

       private:
        std::unique_ptr<componentManager> mComponentManager;
        std::unique_ptr<entityManager> mEntityManager;
        std::unique_ptr<systemManager> mSystemManager;
    };
}  // namespace calmar
