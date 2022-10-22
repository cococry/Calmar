#include "resource_handler.hpp"

#include "calmar/core/asset_pool.hpp"

namespace calmar {
    std::shared_ptr<texture2d> resourceHandler::createTexture(const std::string& filepath, textureFilterMode filterMode) {
        if (assetPool::getTextureWithFilepath(filepath) == nullptr || assetPool::getTextureWithFilepath(filepath)->getData().filterMode != filterMode) {
            std::shared_ptr<texture2d> ret = texture2d::createRef(filepath, filterMode);
            assetPool::addTexture(ret);
            return ret;
        } else {
            return assetPool::getTextureWithFilepath(filepath);
        }
    }
    void resourceHandler::deleteTexture(const std::shared_ptr<texture2d>& texture) {
        assetPool::removeTexture(texture);
    }
}  // namespace calmar
