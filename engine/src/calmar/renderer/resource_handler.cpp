#include "pch.h"
#include "resource_handler.hpp"

#include "calmar/core/asset_pool.hpp"

namespace calmar {
    std::shared_ptr<texture2d> resourceHandler::createTexture(const std::string& filepath, textureFilterMode filterMode) {
        if (assetPool::getTextureWithFilepathAndFilterMode(filepath, filterMode) == nullptr) {
            std::shared_ptr<texture2d> ret = texture2d::createRef(filepath, filterMode);
            assetPool::addTexture(ret);
            return ret;
        } else {
            return assetPool::getTextureWithFilepathAndFilterMode(filepath, filterMode);
        }
    }
    void resourceHandler::deleteTexture(const std::shared_ptr<texture2d>& texture) {
        assetPool::removeTexture(texture);
    }
}  // namespace calmar
