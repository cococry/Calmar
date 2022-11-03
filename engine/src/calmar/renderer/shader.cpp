#include "pch.h"
#include "shader.hpp"

#include "calmar/core/application.hpp"

#include "calmar/platform/opengl/gl_shader.hpp"

namespace calmar {
    shader shader::create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return glShader(vertexPath, fragmentPath, geometryPath);
                break;
            default:
                return shader();
                break;
        }
    }
    std::shared_ptr<shader> shader::createRef(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_shared<glShader>(vertexPath, fragmentPath, geometryPath);
                break;
            default:
                return std::make_shared<shader>();
                break;
        }
    }
    std::unique_ptr<shader> shader::createScoped(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                return std::make_unique<glShader>(vertexPath, fragmentPath, geometryPath);
                break;
            default:
                return std::make_unique<shader>();
                break;
        }
    }
}  // namespace calmar
