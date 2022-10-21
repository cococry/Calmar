#include "event_dispatcher.hpp"

namespace calmar {

    void eventDispatcher::listen(eventType type, const eventCallback& callback) {
        // Inserting the given callback to the callbacks of the given event type
        mEvents[type].push_back(callback);
    }

    void eventDispatcher::dispatch(event ev) {
        // Getting the type of the event
        eventType type = ev.type();

        // Checking if the event type is being listend
        if (mEvents.find(type) == mEvents.end()) return;

        // Callback all of the callbacks linked to the event's type
        for (auto& callback : mEvents[type]) {
            callback(ev);
        }
    }

}  // namespace calmar
