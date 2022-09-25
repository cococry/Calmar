#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/core/application_attachment.hpp"

#include "calmar/renderer/vertex_array.hpp"
#include "calmar/renderer/shader.hpp"
#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/render_command.hpp"

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
        std::shared_ptr<vertexArray> mVertexArray;

        std::shared_ptr<shader> mShader;

        std::shared_ptr<texture2d> mTexture;

        orbitCamera mEditorCamera;
    };

}  // namespace calmarEd
