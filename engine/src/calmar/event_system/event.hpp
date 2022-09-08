#pragma once

#include <functional>

namespace calmar {
    // A simple alias for a const char* to define the type of a evnet
    using eventType = const char*;

    /*  This is a base class that only contains a static member variable to define the type of
        the event (intended to be overwritten by subclasses). */
    class event {
       public:
        /// @brief This virtual constant member function returns the type of the event.
        /// Returns "baseEvent" if the subclass does not overwrite it or the instance of
        /// the class on which this function is called is this class.
        /// @return Returns the constexpr member variable "evType"
        virtual inline eventType type() const {
            return evType;
        }

        // Defines the type of the event (if not overwritten by subclass or called
        // from instance of this base class, it returns "baseEvent")
        static constexpr eventType evType = "baseEvent";
    };

}  // namespace calmar

// A simple macro to make the code more readable
#define COMPARE_EVENTS(ev1, ev2) ev1.type() == ev2::evType

// Binds a target function with std::bind
#define EVENT_CALLBACK(callback) std::bind(&callback, this, std::placeholders::_1)
