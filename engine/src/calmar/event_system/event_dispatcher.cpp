#include "event_dispatcher.hpp"

namespace calmar {

    void eventDispatcher::listen(eventType type, const eventCallback& callback) {
        mEvents[type].push_back(callback);
    }

    void eventDispatcher::dispatch(const event& ev) {
        eventType type = ev.type();

        if (mEvents.find(type) == mEvents.end()) return;

        for (auto& callback : mEvents[type]) {
            callback(ev);
        }
    }

}  // namespace calmar
