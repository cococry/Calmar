#pragma once

#include "ecs_defines.hpp"
#include "component_list.hpp"

#include "calmar/core/asserting.hpp"

#include <unordered_map>
#include <memory>

namespace calmar {
    class CALMAR_API componentManager {
       public:
        template <typename T>
        void registerComponent() {
            const char* typeName = typeid(T).name();

            CALMAR_ASSERT_MSG(mComponentTypes.find(typeName) == mComponentTypes.end(), "Tried to register same component more than once.");

            mComponentTypes.insert({typeName, mNextComponentType});
            mComponentLists.insert({typeName, std::make_shared<componentList<T>>()});
            mNextComponentType++;
        }

        template <typename T>
        componentType getComponentType() {
            const char* typeName = typeid(T).name();

            CALMAR_ASSERT_MSG(mComponentTypes.find(typeName) != mComponentTypes.end(), "Tried to use unregistered component.");

            return mComponentTypes[typeName];
        }

        template <typename T>
        void addComponent(entity entty, T component) {
            getComponentList<T>()->insertData(entty, component);
        }

        template <typename T>
        void removeComponent(entity entty) {
            getComponentList<T>()->removeData(entity);
        }

        template <typename T>
        T& getComponent(entity entty) {
            return getComponentList<T>()->getData(entty);
        }

        void entityDestroyed(entity entty);

       private:
        std::unordered_map<const char*, componentType> mComponentTypes{};

        std::unordered_map<const char*, std::shared_ptr<iComponentList>> mComponentLists{};

        componentType mNextComponentType{};

        template <typename T>
        std::shared_ptr<componentList<T>> getComponentList() {
            const char* typeName = typeid(T).name();

            CALMAR_ASSERT_MSG(mComponentTypes.find(typeName) != mComponentTypes.end(), "Tried to use unregistered component.");

            return std::static_pointer_cast<componentList<T>>(mComponentLists[typeName]);
        }
    };
}  // namespace calmar
