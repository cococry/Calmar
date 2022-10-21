#include "scene.hpp"

#include "ecs_defines.hpp"
#include "ecs.hpp"
#include "components.hpp"
#include "calmar/core/application.hpp"

#include "calmar/renderer/batch_renderer_2d.hpp"
#include "calmar/renderer/entity_camera.hpp"

#include "calmar/renderer/resource_handler.hpp"

namespace calmar {
    scene::scene() {
        mCameraTexture = resourceHandler::createTexture("../editor/assets/icons/camera-icon.png");
    }
    scene::~scene() {
    }
    void scene::init() {
        componentSet listenedComponents;
        listenedComponents.set(ECS.getComponentType<transformComponent>());

        ECS.setSystemComponentSet<scene>(listenedComponents);
    }
    void scene::update() {
        batchRenderer2d::beginRender(application::getInstance()->getEditorAttachment()->camera);
        for (auto const& entity : mEntities) {
            auto const& transform = ECS.getComponent<transformComponent>(entity);
            if (ECS.hasComponent<spriteRendererComponent>(entity)) {
                auto const& spriteRenderer = ECS.getComponent<spriteRendererComponent>(entity);

                if (spriteRenderer.texture != nullptr)
                    batchRenderer2d::renderQuad(transform.position, glm::vec2(transform.scale.x, transform.scale.y), spriteRenderer.texture, spriteRenderer.tint, transform.rotation.z, entity);
                else {
                    batchRenderer2d::renderQuad(transform.position, glm::vec2(transform.scale.x, transform.scale.y), spriteRenderer.tint, transform.rotation.z, entity);
                }
            } else if (ECS.hasComponent<cameraComponent>(entity) && !ECS.hasComponent<spriteRendererComponent>(entity)) {
                batchRenderer2d::renderQuad(transform.position, glm::vec2(1.0f), mCameraTexture, glm::vec4(1.0f), 0.0f, entity);
            }
        }
        batchRenderer2d::endRender();
    }

    void scene::updateRuntime() {
        entityCamera* renderCamera = nullptr;
        glm::mat4 cameraTransform;

        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto& cameraComp = ECS.getComponent<cameraComponent>(entty);
                auto& transformComp = ECS.getComponent<transformComponent>(entty);
                if (cameraComp.selectedForRendering) {
                    renderCamera = &cameraComp.camera;
                    cameraTransform = transformComp.getTransform();
                }
            }
        }

        if (renderCamera) {
            batchRenderer2d::beginRender(*renderCamera, cameraTransform);
            for (auto const& entty : mEntities) {
                if (ECS.hasComponent<transformComponent>(entty)) {
                    auto const& transformComp = ECS.getComponent<transformComponent>(entty);

                    if (ECS.hasComponent<spriteRendererComponent>(entty)) {
                        auto const& spriteRendererComp = ECS.getComponent<spriteRendererComponent>(entty);

                        if (spriteRendererComp.texture) {
                            batchRenderer2d::renderQuad(transformComp.position, glm::vec2(transformComp.scale.x, transformComp.scale.y), spriteRendererComp.texture, spriteRendererComp.tint, transformComp.rotation.z, entty);
                        } else {
                            batchRenderer2d::renderQuad(transformComp.position, glm::vec2(transformComp.scale.x, transformComp.scale.y), spriteRendererComp.tint, transformComp.rotation.z, entty);
                        }
                    }
                }
            }
            batchRenderer2d::endRender();
        }
    }

    entity scene::getRenderCameraEntity() const {
        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto const& cameraComp = ECS.getComponent<cameraComponent>(entty);
                if (cameraComp.selectedForRendering) {
                    return entty;
                }
            }
        }
        return -1;
    }

    std::shared_ptr<scene> scene::copy(std::shared_ptr<scene> sceneToCopy) {
        std::shared_ptr<scene> newScene = std::make_shared<scene>();

        newScene->mEntities = sceneToCopy->mEntities;

        return newScene;
    }

    void scene::handleResize(u32 width, u32 height) {
        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto& cameraComp = ECS.getComponent<cameraComponent>(entty);

                cameraComp.camera.setViewportSize(width, height);
            }
        }
    }
}  // namespace calmar
