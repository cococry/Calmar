#pragma once

#include "ecs_defines.hpp"

#include "calmar/core/asserting.hpp"

#include <array>
#include <unordered_map>

namespace calmar {
    class iComponentList {
       public:
        virtual ~iComponentList() = default;
        virtual void entityDestroyed(entity entty) = 0;
    };

    template <typename T>
    class componentList : public iComponentList {
       public:
        void insertData(entity entty, T component) {
            CALMAR_ASSERT_MSG(mEntityToIndexMap.find(entty) == mEntityToIndexMap.end(), "Tried to add component to same entity more than once.");

            u64 newIndex = mSize;
            mEntityToIndexMap[entty] = newIndex;
            mIndexToEntityMap[newIndex] = entty;
            mComponentArray[newIndex] = component;
            ++mSize;
        }

        void removeData(entity entty) {
            CALMAR_ASSERT_MSG(mEntityToIndexMap.find(entty) != mEntityToIndexMap.end(), "Tried to remove non-existent component.");

            u64 indexOfRemovedEntity = mEntityToIndexMap[entty];
            u64 indexOfLastElement = mSize - 1;
            mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

            entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            mEntityToIndexMap.erase(entty);
            mIndexToEntityMap.erase(indexOfLastElement);

            --mSize;
        }

        T& getData(entity entty) {
            CALMAR_ASSERT_MSG(mEntityToIndexMap.find(entty) != mEntityToIndexMap.end(), "Tried to retrieve non-existent component.");
            return mComponentArray[mEntityToIndexMap[entty]];
        }

        virtual void entityDestroyed(entity entty) override {
            if (mEntityToIndexMap.find(entty) != mEntityToIndexMap.end()) {
                removeData(entty);
            }
        }

       private:
        std::array<T, MAX_ENTITIES> mComponentArray;

        std::unordered_map<entity, u64> mEntityToIndexMap;

        std::unordered_map<u64, entity> mIndexToEntityMap;

        u64 mSize;
    };
}  // namespace calmar
