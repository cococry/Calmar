#pragma once

#include "defines.hpp"

#include "calmar/event_system/event.hpp"

namespace calmar {
    class applicationAttachment {
       public:
        virtual void init() {}

        virtual void update() {}

        virtual void shutdown() {}

        virtual void handleEvents(const event& ev) {}

       private:
    };
}  // namespace calmar
