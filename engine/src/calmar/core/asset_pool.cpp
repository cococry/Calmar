#include "pch.h"
#include "asset_pool.hpp"

namespace calmar {
    std::vector<std::shared_ptr<texture2d>> assetPool::textures;

    void assetPool::addTexture(const std::shared_ptr<texture2d>& texture) {
        textures.emplace_back(texture);
    }
    void assetPool::removeTexture(const std::shared_ptr<texture2d>& texture) {
        auto it = std::find(textures.begin(), textures.end(), texture);

        if (it != textures.end()) {
            textures.erase(it);
        }
    }
    bool assetPool::hasTexture(const std::shared_ptr<texture2d>& texture) {
        return std::find(textures.begin(), textures.end(), texture) != textures.end();
    }

    std::shared_ptr<texture2d> assetPool::getTextureWithFilepath(const std::string& filepath) {
        for (const auto& texture : textures) {
            if (texture->getData().filepath == filepath) {
                return texture;
            }
        }
        return nullptr;
    }

    std::shared_ptr<texture2d> assetPool::getTextureWithFilepathAndFilterMode(const std::string& filepath, textureFilterMode filterMode) {
        for (const auto& texture : textures) {
            if (texture->getData().filepath == filepath && texture->getData().filterMode == filterMode) {
                return texture;
            }
        }
        return nullptr;
    }
    std::shared_ptr<texture2d> assetPool::getTexture(u32 index) {
        return textures[index];
    }
}  // namespace calmar
