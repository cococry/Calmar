#pragma once

namespace calmar {
    /* A simple enumaration to define different rendering backends*/
    enum class renderingBackend {
        NONE = 0,
        OPENGL,
        VULKAN,
        DIRECT3D
    };
}  // namespace calmar
