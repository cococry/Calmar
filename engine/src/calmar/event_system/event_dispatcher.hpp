#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "event.hpp"

#include "calmar/core/defines.hpp"

namespace calmar {
    // A simple alias for an std::function<void(const event ev)>
    using eventCallback = std::function<void(event)>;

    /* This class is used to to listen given events and dispatch events
        This is done by storing a unordered map in the class which links a event type to a
        event callback. The "listen()" method adds a element to this map. The dispatch method
        takes in an event and checks if the event is being listened. If it is, it executes the
        the callback which is linked with the event type of the given event.*/
    class eventDispatcher {
       public:
        /// @brief Listens to a given type of event and links a given function with it. In summary, The
        /// method adds a new elment to the events which the class is listening to.
        /// @param type The type of event to which the dispatcher should listen
        /// @param callback The callback that is called when the given type of event was
        // recived.
        void listen(eventType type, const eventCallback& callback);

        /// @brief Checks if the given event type is listend and if it is, it calls the call back that is linked
        /// to its time.
        /// @param ev The event which is being dispatched
        void dispatch(event ev);

       private:
        std::unordered_map<eventType, std::vector<eventCallback>> mEvents;
    };

}  // namespace calmar
