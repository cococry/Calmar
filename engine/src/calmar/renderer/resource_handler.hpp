#pragma once

#include "calmar/core/defines.hpp"
#include "texture.hpp"

#include <memory>

namespace calmar {
    class resourceHandler {
       public:
        static std::shared_ptr<texture2d> createTexture(const std::string& filepath, textureFilterMode filterMode = textureFilterMode::Linear);

        static void deleteTexture(const std::shared_ptr<texture2d>& texture);
    };
}  // namespace calmar
