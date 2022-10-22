#include "indexed_atlas_texture.hpp"

#include "calmar/core/logging.hpp"

namespace calmar {
    indexedAtlasTexture::indexedAtlasTexture(const std::shared_ptr<texture2d>& texture, const glm::vec2& min, const glm::vec2& max)
        : atlasTexture(texture) {
        mTexCoord[0] = {min.x, min.y};
        mTexCoord[1] = {max.x, min.y};
        mTexCoord[2] = {max.x, max.y};
        mTexCoord[3] = {min.x, max.y};
    }

    std::shared_ptr<indexedAtlasTexture> indexedAtlasTexture::createWithCoords(const std::shared_ptr<texture2d>& texture, const glm::vec2& coords, const glm::vec2& cellSize) {
        CALMAR_INFO("Cellsize: {0}, {1}", cellSize.x, cellSize.y);
        CALMAR_INFO("Coords: {0}, {1}", coords.x, coords.y);
        CALMAR_INFO("Texture: {0}, {1}", texture->getData().width, texture->getData().height);

        glm::vec2 min = {(float)((coords.x * cellSize.x) / texture->getData().width), (float)((coords.y * cellSize.y) / texture->getData().height)};
        glm::vec2 max = {((float)((coords.x + 1) * cellSize.x) / texture->getData().width), (float)(((coords.y + 1) * cellSize.y) / texture->getData().height)};
        CALMAR_INFO("{0}, {1}", min.x, min.y);
        CALMAR_INFO("{0}, {1}", max.x, max.y);
        return std::make_shared<indexedAtlasTexture>(texture, min, max);
    }
}  // namespace calmar
