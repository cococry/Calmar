#pragma once

#include "calmar/renderer/rendering_backend.hpp"
#include "calmar/core/window.hpp"

namespace calmar {
    struct globalState {
        windowingBackend windowBackend;
        renderingBackend renderBackend;
    };

    static globalState gState;
}  // namespace calmar
