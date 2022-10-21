#pragma once

#include "calmar/renderer/framebuffer.hpp"

namespace calmar {
    class glFrameBuffer : public framebuffer {
       public:
        glFrameBuffer(const framebufferProperties& props);

        ~glFrameBuffer();

        virtual void reConstruct() override;

        virtual void resize(u32 width, u32 height) override;

        virtual void bind() override;

        virtual void unbind() override;

        virtual void clearAttachment(u32 attachmentIndex, i32 data) override;

        virtual framebufferProperties getProperties() const override {
            return mProps;
        }

        virtual i32 readPixel(u32 attachmentIndex, i32 x, i32 y) override;

       private:
        framebufferProperties mProps;
    };
}  // namespace calmar
