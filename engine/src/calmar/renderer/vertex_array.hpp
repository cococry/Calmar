#pragma once

#include "calmar/core/defines.hpp"

#include "render_buffers.hpp"

#include <memory>

#include <vector>

#include <glad/glad.h>

#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

namespace calmar {
    struct renderDataTypes {
       public:
        static void init();

       private:
    };
    class vertexArray {
       public:
        static const vertexArray& create();

        static const std::shared_ptr<vertexArray>& createRef();

        static const std::unique_ptr<vertexArray>& createScoped();

        virtual void bind() {}

        virtual void unbind() {}

        virtual void deleteId() {}

        virtual void setVertexLayoutAttribute(u32 index);

        inline const std::vector<std::shared_ptr<vertexBuffer>>& getVertexBuffers() const {
            return mVertexBuffers;
        }

        inline const std::shared_ptr<indexBuffer>& getIndexBuffer() const {
            return mIndexBufer;
        }

        inline u32 getAttribOffset() const {
            return mAttribOffset;
        }

        inline u32 getAttribIndex() const {
            return mAttribIndex;
        }

        inline u32 geVertexStride() const {
            return mVertexStride;
        }

       protected:
        render_id mId;

        std::vector<std::shared_ptr<vertexBuffer>> mVertexBuffers;
        std::shared_ptr<indexBuffer> mIndexBufer;

        u32 mAttribOffset = 0;
        u32 mAttribIndex = 0;
        u32 mVertexStride = 0;
    };
}  // namespace calmar
