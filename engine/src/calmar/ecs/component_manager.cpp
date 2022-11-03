#include "pch.h"
#include "component_manager.hpp"

namespace calmar {
    void componentManager::entityDestroyed(entity entty) {
        for (auto const& pair : mComponentLists) {
            auto const& component = pair.second;

            component->entityDestroyed(entty);
        }
    }

}  // namespace calmar
