#include "pch.h"
#include "gl_rendering.hpp"

#include <glad/glad.h>

namespace calmar {
    void glRendering::init() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    const u8* glRendering::getVersion() {
        return glGetString(GL_VERSION);
    }
}  // namespace calmar
