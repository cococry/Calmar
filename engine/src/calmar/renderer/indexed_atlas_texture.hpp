#pragma once

#include "calmar/core/defines.hpp"

#include "texture.hpp"

#include <memory>

#include <glm/glm.hpp>

namespace calmar {
    class indexedAtlasTexture {
       public:
        indexedAtlasTexture() = default;

        indexedAtlasTexture(const std::shared_ptr<texture2d>& texture, const glm::vec2& min, const glm::vec2& max);

        const glm::vec2* getTextureCoords() const { return mTexCoord; }

        static std::shared_ptr<indexedAtlasTexture> createWithCoords(const std::shared_ptr<texture2d>& texture, const glm::vec2& coords, const glm::vec2& cellSize);

        std::shared_ptr<texture2d> atlasTexture;

       private:
        glm::vec2 mTexCoord[4];
    };
}  // namespace calmar
