#pragma once

#include <calmar/core/application_attachment.hpp>
#include <calmar/renderer/texture.hpp>
#include <calmar/ecs/scene.hpp>
#include <calmar/ecs/ecs.hpp>

#include <calmar/renderer/indexed_atlas_texture.hpp>

using namespace calmar;

namespace calmarEd {
    class sceneHirarchyPanel {
       public:
        void init(const std::shared_ptr<scene>& scene);

        void update();

        void renderImGui();

        inline entity getSelectedEntity() const {
            return mSelectedEntity;
        }
        inline void setSelectedEntity(entity entty) {
            mSelectedEntity = entty;
        }

        entity duplicateEntity(entity& entty);

        inline void setScene(const std::shared_ptr<scene>& scene) {
            mScene = scene;
            mSelectedEntity = -1;
        }

       private:
        void renderImGuiEntityNode(entity entty);

        void renderImGuiEntityComponents(entity entty);

        void renderImGuiVec3Slider(const std::string& label, glm::vec3& vec, entity entty, float resetValue = 0.0f);

        void handleInput();

        template <typename T>
        void duplicateComponentIfHas(entity source, entity dest);

        std::shared_ptr<scene> mScene;
        std::shared_ptr<texture2d> mDefaultTexture;
        std::shared_ptr<indexedAtlasTexture> mPreviewSubtexture;
        bool mChangedSubtexture = false;

        entity mSelectedEntity = -1;
        entity mDeletedEntity = 1;
    };
}  // namespace calmarEd
