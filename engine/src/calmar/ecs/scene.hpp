#pragma once

#include "system.hpp"

#include <memory>

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
    };
}  // namespace calmar
