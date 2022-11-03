#include "pch.h"
#include "indexed_atlas_texture.hpp"

namespace calmar {
    indexedAtlasTexture::indexedAtlasTexture(const std::shared_ptr<texture2d>& texture, const glm::vec2& min, const glm::vec2& max)
        : atlasTexture(texture) {
        mTexCoord[0] = {min.x, min.y};
        mTexCoord[1] = {max.x, min.y};
        mTexCoord[2] = {max.x, max.y};
        mTexCoord[3] = {min.x, max.y};
    }

    std::shared_ptr<indexedAtlasTexture> indexedAtlasTexture::createWithCoords(const std::shared_ptr<texture2d>& texture, const glm::vec2& coords, const glm::vec2& cellSize) {
        glm::vec2 min = {(float)((coords.x * cellSize.x) / texture->getData().width), (float)((coords.y * cellSize.y) / texture->getData().height)};
        glm::vec2 max = {((float)((coords.x + 1) * cellSize.x) / texture->getData().width), (float)(((coords.y + 1) * cellSize.y) / texture->getData().height)};

        return std::make_shared<indexedAtlasTexture>(texture, min, max);
    }
}  // namespace calmar
