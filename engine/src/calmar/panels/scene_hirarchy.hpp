#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/core/application_attachment.hpp"

namespace calmar {
    class sceneHirarchyPanel : public applicationAttachment {
       public:
        virtual void init() override;

        virtual void update() override;

        virtual void renderImGui() override;

       private:
    };
}  // namespace calmar
