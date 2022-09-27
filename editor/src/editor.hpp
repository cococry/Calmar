#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/core/application_attachment.hpp"

#include "calmar/renderer/texture.hpp"

#include "calmar/renderer/orbit_camera.hpp"

#include <memory>

using namespace calmar;

namespace calmarEd {
    class editorAttachment : public applicationAttachment {
       public:
        editorAttachment() = default;

        virtual void init() override;

        virtual void update() override;

        virtual void shutdown() override;

        virtual void handleEvents(const event& ev) override;

       private:
        orbitCamera mEditorCamera;

        std::shared_ptr<texture2d> mCppLogoTexture;
        std::shared_ptr<texture2d> mCalmarLogoTexture;
        std::shared_ptr<texture2d> mFoliageTexture;
    };

}  // namespace calmarEd
