#pragma once

#include "ecs_defines.hpp"
#include "system.hpp"

#include "calmar/core/asserting.hpp"

#include <memory>
#include <unordered_map>

namespace calmar {
    class systemManager {
       public:
        template <typename T>
        std::shared_ptr<T> registerSystem() {
            const char* typeName = typeid(T).name();

            CALMAR_ASSERT_MSG(mSystems.find(typeName) == mSystems.end(), "Tried to register the same system more than once.");

            auto system = std::make_shared<T>();
            mSystems.insert({typeName, system});
            return system;
        }

        template <typename T>
        void setComponentSet(componentSet set) {
            const char* typeName = typeid(T).name();

            CALMAR_ASSERT_MSG(mSystems.find(typeName) != mSystems.end(), "Tried to use unregistered system.");

            // Set the signature for this system
            mCompoentSets.insert({typeName, set});
        }

        void entityDestroyed(entity entty);

        void entityComponentSetChanged(entity entty, componentSet set);

       private:
        std::unordered_map<const char*, componentSet> mCompoentSets{};

        std::unordered_map<const char*, std::shared_ptr<systemEcs>> mSystems;
    };
}  // namespace calmar
