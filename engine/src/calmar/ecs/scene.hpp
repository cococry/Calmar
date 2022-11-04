#pragma once

#include "system.hpp"

#include "calmar/renderer/texture.hpp"

#include "components.hpp"

class b2World;

namespace calmar {

    class scene : public systemEcs {
       public:
        scene() = default;

        scene(bool setComponentSet);

        void init();

        ~scene();

        void update();

        void updateRuntime();

        void handleResize(u32 width, u32 height);

        entity getRenderCameraEntity() const;

        static std::shared_ptr<scene> copy(std::shared_ptr<scene> sceneToCopy);

        void onRuntimeStart();

        void onRuntimeStop();

        i32 velocityIterations = 6;
        i32 positionIterations = 2;
        float gravity = -9.81f;

       private:
        std::vector<transformComponent> mEditorTransforms;

        std::shared_ptr<texture2d> mCameraTexture;

        b2World* mPhysicsWorld = nullptr;
    };
}  // namespace calmar
