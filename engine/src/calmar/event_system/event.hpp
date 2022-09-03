#pragma once

#include <functional>

namespace calmar {
    using eventType = const char*;
    class event {
       public:
        virtual inline eventType type() const {
            return evType;
        }

        static constexpr eventType evType = "baseEvent";
    };

}  // namespace calmar

#define COMPARE_EVENTS(ev1, ev2) ev1.type() == ev2::evType

#define EVENT_CALLBACK(callback) std::bind(&callback, this, std::placeholders::_1)
