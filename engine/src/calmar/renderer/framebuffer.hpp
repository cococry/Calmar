#pragma once

namespace calmar {
    enum class framebufferTextureFormat {
        NONE = 0,
        RGBA8,
        SHADER_RED_INT,
        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };
    struct framebufferTextureProperties {
        framebufferTextureProperties() = default;
        framebufferTextureProperties(framebufferTextureFormat format)
            : textureFormat(format) {}

        framebufferTextureFormat textureFormat = framebufferTextureFormat::NONE;
    };
    struct framebufferAttachmentProperties {
        framebufferAttachmentProperties() = default;
        framebufferAttachmentProperties(std::initializer_list<framebufferTextureProperties> _attachments)
            : attachments(_attachments) {}

        std::vector<framebufferTextureProperties> attachments;
    };
    struct framebufferProperties {
        u32 width, height;
        framebufferAttachmentProperties attachments;
        u32 samples = 1;

        bool renderToSwapchain = false;
    };

    class framebuffer {
       public:
        framebuffer() = default;
        virtual ~framebuffer() {}
        virtual framebufferProperties getProperties() const { return framebufferProperties(); };

        static framebuffer create(const framebufferProperties& props);

        static std::unique_ptr<framebuffer> createScoped(const framebufferProperties& props);

        static std::shared_ptr<framebuffer> createRef(const framebufferProperties& props);

        virtual void reConstruct() {}

        virtual void resize(u32 width, u32 height) {}

        virtual void bind() {}

        virtual void unbind() {}

        virtual void clearAttachment(u32 attachmentIndex, i32 data) {}

        virtual i32 readPixel(u32 attachmentIndex, i32 x, i32 y) { return -1; }

        inline render_id getd() const {
            return mId;
        }

        inline render_id getColorAttachmentId(u32 index = 0) const {
            return mColorAttachments[index];
        }

        inline render_id getDepthAttachmentId() const {
            return mDepthAttachment;
        }

       protected:
        render_id mId = 0;

        std::vector<framebufferTextureProperties> mColorAttachmentProps;
        framebufferTextureProperties mDepthAttachmentProps = framebufferTextureFormat::NONE;

        std::vector<render_id> mColorAttachments;
        u32 mDepthAttachment;
    };
}  // namespace calmar
