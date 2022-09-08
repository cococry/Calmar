#pragma once

#include "calmar/core/defines.hpp"

#include "rendering_backend.hpp"

namespace calmar {
    /* This is the general rendering class in calmar. It is not static and needs to
        be instantiated. THe class contains essential functionally for rendering. It
        binds all of the rendering subsystems together and abstracts away rendering backend code.*/
    class renderer {
       public:
        /// @brief Initializes all of the subsystems that are needed for rendering
        /// @param backend The backend to render with
        void initSubsystems(const renderingBackend& backend);

       private:
    };
}  // namespace calmar
