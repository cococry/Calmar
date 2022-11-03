#pragma once

#include "defines.hpp"

#include "calmar/event_system/event.hpp"

namespace calmar {
    /// @brief This class is used to add functionality to the application
    /// class for abstraction purposes. The class is intented to be inherented
    /// by subclasses and to implement a specific system or layer into the application.
    class applicationAttachment {
       public:
        /// @brief Virtual function to initialize the attachment
        virtual void init() {}

        /// @brief Virtual function to update the attachment
        virtual void update() {}

        /// @brief Virtual function to clean up the attachment's state
        virtual void shutdown() {}

        /// @brief Virtual function to handle events on the attachment
        /// @param ev The event to handle
        virtual void handleEvents(event ev) {}

        /// @brief Virtual function to render imgui panels in the attachment
        virtual void renderImGui() {}

       private:
    };
}  // namespace calmar
