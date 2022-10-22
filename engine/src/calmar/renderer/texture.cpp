#include "texture.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/gl_texture.hpp"

namespace calmar {

    texture2d texture2d::create(const std::string& filepath, textureFilterMode filterMode) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glTexture2d(filepath, filterMode);
                break;
            default:
                return texture2d();
                break;
        }
        return texture2d();
    }
    std::shared_ptr<texture2d> texture2d::createRef(const std::string& filepath, textureFilterMode filterMode) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glTexture2d>(filepath, filterMode);
                break;
            default:
                return std::make_shared<texture2d>();
                break;
        }
        return std::make_shared<texture2d>();
    }
    std::unique_ptr<texture2d> texture2d::createScoped(const std::string& filepath, textureFilterMode filterMode) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glTexture2d>(filepath, filterMode);
                break;
            default:
                return std::make_unique<texture2d>();
                break;
        }
        return std::make_unique<texture2d>();
    }
    texture2d texture2d::create(u32 width, u32 height) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glTexture2d(width, height);
                break;
            default:
                return texture2d();
                break;
        }
        return texture2d();
    }
    std::shared_ptr<texture2d> texture2d::createRef(u32 width, u32 height) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glTexture2d>(width, height);
                break;
            default:
                return std::make_shared<texture2d>();
                break;
        }
        return std::make_shared<texture2d>();
    }
    std::unique_ptr<texture2d> texture2d::createScoped(u32 width, u32 height) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glTexture2d>(width, height);
                break;
            default:
                return std::make_unique<texture2d>();
                break;
        }
        return std::make_unique<texture2d>();
    }
}  // namespace calmar
