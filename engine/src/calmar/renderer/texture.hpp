#pragma once

#include "calmar/core/defines.hpp"

#include <string>
#include <memory>

namespace calmar {
    struct texture2dObject {
        u32 width, height;
        u32 numberOfChannels;
        u32 dataFormat, internalFormat;
        std::string filepath;
    };

    class CALMAR_API texture2d {
       public:
        static texture2d create(const std::string& filepath);

        static std::shared_ptr<texture2d> createRef(const std::string& filepath);

        static std::unique_ptr<texture2d> createScoped(const std::string& filepath);
        texture2d() = default;
        virtual ~texture2d() {}

        virtual void bind() {}

        virtual void activateSlot(u32 slot = 0) {}

        virtual void unbind() {}

        virtual void deleteId() {}

        inline render_id getId() const {
            return mId;
        }

        inline const texture2dObject& getData() const {
            return mTextureData;
        }

       protected:
        render_id mId;
        texture2dObject mTextureData;
    };
}  // namespace calmar
