#pragma once

#include "calmar/core/defines.hpp"

#include "event.hpp"

namespace calmar {

    class keyEvent : public event {
       public:
        keyEvent(u32 key)
            : mKey(key) {
        }

        inline u32 getKey() const {
            return mKey;
        }

        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyEvent";

       protected:
        u32 mKey;
    };

    class keyPressedEvent : public keyEvent {
       public:
        keyPressedEvent(u32 key)
            : keyEvent(key) {
        }
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyPressedEvent";

       private:
    };

    class keyReleasedEvent : public keyEvent {
       public:
        keyReleasedEvent(u32 key)
            : keyEvent(key) {
        }
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyReleasedEvent";

       private:
    };

}  // namespace calmar
