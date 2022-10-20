#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/entity_camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

namespace calmar {
    struct tagComponent {
        tagComponent()
            : tag("Entity") {
        }

        tagComponent(const std::string& tag)
            : tag(tag) {
        }

        std::string tag;
    };
    struct transformComponent {
        transformComponent() {
            position = glm::vec3(0.0f);
            rotation = glm::vec3(0.0f),
            scale = glm::vec3(1.0f);
        }
        transformComponent(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
            : position(_position), rotation(_rotation), scale(_scale) {}
        ~transformComponent() {
        }

        glm::mat4 getTransform() const {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));

            return glm::translate(glm::mat4(1.0f), position) * rot * glm::scale(glm::mat4(1.0f), scale);
        };
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
    struct spriteRendererComponent {
        spriteRendererComponent() {
            texture = nullptr;
            tint = glm::vec4(1.0f);
        }

        spriteRendererComponent(const std::shared_ptr<texture2d>& _texture, const glm::vec4& _tint = glm::vec4(1.0f))
            : texture(_texture), tint(_tint) {
        }

        std::shared_ptr<texture2d> texture;
        glm::vec4 tint;
    };

    struct cameraComponent {
        entityCamera camera;
        bool selectedForRendering = true;

        cameraComponent() = default;
        cameraComponent(const cameraComponent&) = default;
    };
}  // namespace calmar
