#include "scene.hpp"

#include "ecs_defines.hpp"
#include "ecs.hpp"
#include "components.hpp"
#include "calmar/core/application.hpp"

#include "calmar/renderer/batch_renderer_2d.hpp"

namespace calmar {
    scene::scene() {
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
                    batchRenderer2d::renderQuad(transform.position, glm::vec2(transform.scale.x, transform.scale.y), spriteRenderer.texture, spriteRenderer.tint, transform.rotation.z);
                else
                    batchRenderer2d::renderQuad(transform.position, glm::vec2(transform.scale.x, transform.scale.y), spriteRenderer.tint, transform.rotation.z);
            }
        }
    }
}  // namespace calmar
