#include "pch.h"
#include "system_manager.hpp"

namespace calmar {
    void systemManager::entityDestroyed(entity entty) {
        for (auto const& pair : mSystems) {
            auto const& system = pair.second;
            system->mEntities.erase(entty);
        }
    }

    void systemManager::entityComponentSetChanged(entity entty, componentSet set) {
        for (auto const& pair : mSystems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = mCompoentSets[type];

            if ((set & systemSignature) == systemSignature) {
                system->mEntities.insert(entty);
            } else {
                system->mEntities.erase(entty);
            }
        }
    }
}  // namespace calmar
