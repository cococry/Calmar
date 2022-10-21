#pragma once

#include "system.hpp"

#include <memory>
#include "calmar/renderer/texture.hpp"

namespace calmar {
    class scene : public systemEcs {
       public:
        scene();

        ~scene();

        void init();

        void update();

        void updateRuntime();

        void handleResize(u32 width, u32 height);

        entity getRenderCameraEntity() const;

        static std::shared_ptr<scene> copy(std::shared_ptr<scene> sceneToCopy);

       private:
        std::shared_ptr<texture2d> mCameraTexture;
    };
}  // namespace calmar
