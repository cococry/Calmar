#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "event.hpp"

namespace calmar {
    using eventCallback = std::function<void(const event& ev)>;

    class eventDispatcher {
       public:
        void listen(eventType type, const eventCallback& callback);

        void dispatch(const event& ev);

       private:
        std::unordered_map<eventType, std::vector<eventCallback>> mEvents;
    };

}  // namespace calmar
