#pragma once

#include "pch.h"

#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/indexed_atlas_texture.hpp"
#include "calmar/renderer/entity_camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));

            return glm::translate(glm::mat4(1.0f), position) * rot * glm::scale(glm::mat4(1.0f), scale);
        };
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        transformComponent(const transformComponent&) = default;
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

        spriteRendererComponent(const spriteRendererComponent&) = default;
    };

    struct cameraComponent {
        entityCamera camera;
        bool selectedForRendering = true;

        cameraComponent() = default;
        cameraComponent(const cameraComponent&) = default;
    };

    struct indexedTextureComponent {
       public:
        std::shared_ptr<indexedAtlasTexture> indexedTexture = nullptr;
        glm::vec4 tint = glm::vec4(1.0f);
        glm::vec2 coordsOnSheet = glm::vec2(0);
        glm::vec2 cellSize = glm::vec2(0);
        std::shared_ptr<texture2d> atlasTexture = nullptr;
        textureFilterMode atlasTextureFilterMode = textureFilterMode::Linear;

        indexedTextureComponent(const std::shared_ptr<indexedAtlasTexture>& texture, const glm::vec4& tint)
            : indexedTexture(texture), tint(tint) {
        }

        indexedTextureComponent() = default;
        indexedTextureComponent(const indexedTextureComponent&) = default;

       private:
    };

    struct rigidBody2dComponent {
        enum class bodyType { Static = 0,
                              Dynamic };

        bodyType type = bodyType::Static;
        bool fixedRotation = false;

        void* runtimeBody = nullptr;

        rigidBody2dComponent() = default;
        rigidBody2dComponent(const rigidBody2dComponent&) = default;
    };
    struct boxCollider2dComponent {
        glm::vec2 offset = {0.0f, 0.0f};
        glm::vec2 size = {0.5f, 0.5f};

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;

        void* runtimeFixture = nullptr;

        boxCollider2dComponent() = default;
        boxCollider2dComponent(const boxCollider2dComponent&) = default;
    };

    struct cSharpScriptComponent {
        std::string name;

        cSharpScriptComponent() = default;
        cSharpScriptComponent(const cSharpScriptComponent&) = default;
    };

}  // namespace calmar
