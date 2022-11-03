#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/core/application_attachment.hpp"

#include <imgui.h>

namespace calmar {
    class imGuiHandler : public applicationAttachment {
       public:
        imGuiHandler();

        virtual ~imGuiHandler();

        virtual void init() override;

        virtual void shutdown() override;

        virtual void renderImGui() override;

        void beginImGui();

        void endImGui();

       private:
    };
}  // namespace calmar
