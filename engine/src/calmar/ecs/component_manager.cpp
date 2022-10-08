#include "component_manager.hpp"

#include "calmar/core/asserting.hpp"

namespace calmar {
    void componentManager::entityDestroyed(entity entty) {
        for (auto const& pair : mComponentLists) {
            auto const& component = pair.second;

            component->entityDestroyed(entty);
        }
    }

}  // namespace calmar
