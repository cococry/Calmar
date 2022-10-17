#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/renderer/texture.hpp"

#include <memory>
#include <vector>
#include <string>

namespace calmar {
    class assetPool {
       public:
        static void addTexture(const std::shared_ptr<texture2d>& texture);

        static void removeTexture(const std::shared_ptr<texture2d>& texture);

        static bool hasTexture(const std::shared_ptr<texture2d>& texture);

        static std::shared_ptr<texture2d> getTextureWithFilepath(const std::string& filepath);

        static std::shared_ptr<texture2d> getTexture(u32 index);

        static std::vector<std::shared_ptr<texture2d>> textures;
    };
}  // namespace calmar
