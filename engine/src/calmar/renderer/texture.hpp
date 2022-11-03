#pragma once

namespace calmar {
    enum class textureFilterMode {
        Nearest = 0,
        Linear
    };
    struct texture2dObject {
        u32 width, height;
        u32 numberOfChannels;
        u32 dataFormat, internalFormat;
        std::string filepath;
        textureFilterMode filterMode;
    };

    class texture2d {
       public:
        static texture2d create(const std::string& filepath, textureFilterMode filterMode = textureFilterMode::Linear);

        static std::shared_ptr<texture2d> createRef(const std::string& filepath, textureFilterMode filterMode = textureFilterMode::Linear);

        static std::unique_ptr<texture2d> createScoped(const std::string& filepath, textureFilterMode filterMode = textureFilterMode::Linear);

        static texture2d create(u32 width, u32 height);

        static std::shared_ptr<texture2d> createRef(u32 width, u32 height);

        static std::unique_ptr<texture2d> createScoped(u32 width, u32 height);
        texture2d() = default;
        virtual ~texture2d() {}

        virtual void bind() {}

        virtual void bindUnit(u32 slot) {}

        virtual void activateSlot(u32 slot = 0) {}

        virtual void unbind() {}

        virtual void deleteId() {}

        virtual void setData(void* data, u32 size) {}

        virtual bool operator==(const texture2d& other) const {
            return false;
        }

        inline render_id getId() const {
            return mId;
        }

        inline const texture2dObject& getData() const {
            return mTextureData;
        }
        inline void setFilterMode(textureFilterMode filterMode) {
            mTextureData.filterMode = filterMode;
        }

       protected:
        render_id mId;
        texture2dObject mTextureData;
    };
}  // namespace calmar
