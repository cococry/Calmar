#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/renderer/vertex_array.hpp"

namespace calmar {

    class glVertexArray : public vertexArray {
       public:
        glVertexArray(u32 vertexStride);

        ~glVertexArray();

        virtual void bind() override;

        virtual void unbind() override;

        virtual void deleteId() override;

        virtual void setVertexLayoutAttribute(layoutAttributeType dataTypeCount, u32 stride = 0, u32 type = renderDataTypes::float32, bool normalized = false) override;

        inline u32 getAttribOffset() const {
            return mAttribOffset;
        }

        inline u32 getAttribIndex() const {
            return mAttribIndex;
        }

        inline u32 geVertexStride() const {
            return mVertexStride;
        }

       private:
        u32 mAttribOffset = 0;
        u32 mAttribIndex = 0;
        u32 mVertexStride = 0;
    };
}  // namespace calmar
