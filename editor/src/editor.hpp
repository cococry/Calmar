#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/core/application_attachment.hpp"
#include "calmar/core/application.hpp"

#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/batch_renderer_2d.hpp"
#include "calmar/renderer/orbit_camera.hpp"

#include "calmar/ecs/ecs.hpp"
#include "calmar/ecs/components.hpp"

#include <memory>

using namespace calmar;

namespace calmarEd {
    class renderSystem : public systemEcs {
       public:
        void render() {
            for (auto const& entity : mEntities) {
                auto const& transform = application::getInstance()->entityComponentSystem.getComponent<transformComponent>(entity);
                auto const& spriteRenderer = application::getInstance()->entityComponentSystem.getComponent<spriteRendererComponent>(entity);

                batchRenderer2d::renderQuad(transform.position, transform.scale, spriteRenderer.texture, spriteRenderer.tint, transform.rotation.z);
            }
        }

       private:
    };
    class editorAttachment : public applicationAttachment {
       public:
        editorAttachment() = default;

        virtual void init() override;

        virtual void update() override;

        virtual void shutdown() override;

        virtual void handleEvents(const event& ev) override;

        virtual void renderImGui() override;

       private:
        std::shared_ptr<texture2d> mGrassTexture;
        std::shared_ptr<texture2d> mPlayerTexture;
        std::shared_ptr<texture2d> mTreeTexture;

        std::shared_ptr<renderSystem> mRenderingSystem;

        orbitCamera mEditorCamera;
    };

}  // namespace calmarEd
