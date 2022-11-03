#pragma once

#include "calmar/renderer/render_buffers.hpp"

namespace calmar {
    class glVertexBuffer : public vertexBuffer {
       public:
        glVertexBuffer(float* data, u32 size, u32 bufferFlags);

        glVertexBuffer(u32 size, u32 bufferFlags);

        ~glVertexBuffer();

        virtual void bind() override;

        virtual void unbind() override;

        virtual void deleteId() override;

        virtual void setData(const void* data, u32 size, u32 offset = 0) override;

       private:
    };

    class glIndexBuffer : public indexBuffer {
       public:
        glIndexBuffer(u32* data, u32 count, u32 bufferFlags);

        ~glIndexBuffer();

        virtual void bind() override;

        virtual void unbind() override;

        virtual void deleteId() override;

       private:
    };

}  // namespace calmar
