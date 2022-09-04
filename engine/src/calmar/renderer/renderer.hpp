#pragma once

#include "calmar/core/defines.hpp"

#include "rendering_backend.hpp"

namespace calmar {
    class renderer {
       public:
        void initSubsystems(const renderingBackend& backend);

       private:
    };
}  // namespace calmar
