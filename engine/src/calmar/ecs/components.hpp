#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/renderer/texture.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace calmar {
    struct CALMAR_API transformComponent {
        transformComponent() {
            position = glm::vec3(1.0f);
            rotation = glm::vec3(0.0f),
            scale = glm::vec3(1.0f);
        }
        transformComponent(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
            : position(_position), rotation(_rotation), scale(_scale) {}
        ~transformComponent() {
        }
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    struct CALMAR_API spriteRendererComponent {
        spriteRendererComponent() {
            texture = std::make_shared<texture2d>();
            tint = glm::vec4(1.0f);
        }

        spriteRendererComponent(const std::shared_ptr<texture2d>& _texture, const glm::vec4& _tint = glm::vec4(1.0f))
            : texture(_texture), tint(_tint) {
        }

        std::shared_ptr<texture2d> texture;
        glm::vec4 tint;
    };
}  // namespace calmar
