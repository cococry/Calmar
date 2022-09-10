#pragma once

#include "calmar/core/defines.hpp"

#include "rendering_backend.hpp"

#include <memory>

namespace calmar {
    struct renderBufferFlags {
        static void init(const renderingBackend& backend);

        static u32 dynamicDraw;
        static u32 staticDraw;
    };

    class vertexBuffer {
       public:
        static const vertexBuffer& create(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        static const vertexBuffer& create(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        static const std::shared_ptr<vertexBuffer>& createRef(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        static const std::shared_ptr<vertexBuffer>& createRef(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        static const std::unique_ptr<vertexBuffer>& createScoped(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        static const std::unique_ptr<vertexBuffer>& createScoped(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        virtual void bind() {}

        virtual void unbind() {}

        virtual void deleteId() {}

        inline render_id getId() const {
            return mId;
        }

        virtual void setData(float* data, u32 size, u32 offset = 0) {}

       protected:
        render_id mId;
    };

    class indexBuffer {
       public:
        static const indexBuffer& create(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        static const std::shared_ptr<indexBuffer>& createRef(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        static const std::unique_ptr<indexBuffer>& createScoped(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        virtual void bind() {}

        virtual void unbind() {}

        virtual void deleteId() {}

        inline render_id getId() const {
            return mId;
        }

        inline u32 getCount() const {
            return mCount;
        }

       protected:
        render_id mId;
        u32 mCount;
    };

}  // namespace calmar
