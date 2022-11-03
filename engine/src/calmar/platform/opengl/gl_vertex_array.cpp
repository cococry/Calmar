#include "pch.h"
#include "gl_vertex_array.hpp"

namespace calmar {
    glVertexArray::glVertexArray(u32 vertexStride) {
        // Setting the member variable vertex stride
        mVertexStride = vertexStride;

        /* Creating the OpenGL vertex array */
        glCreateVertexArrays(1, &mId);
        glBindVertexArray(mId);
    }

    glVertexArray::~glVertexArray() {
        // Deleting the array at destruction
        glDeleteVertexArrays(1, &mId);
    }

    void glVertexArray::bind() {
        glBindVertexArray(mId);
    }

    void glVertexArray::unbind() {
        glBindVertexArray(0);
    }

    void glVertexArray::deleteId() {
        glDeleteVertexArrays(1, &mId);
    }

    void glVertexArray::setVertexLayoutAttribute(layoutAttributeType dataTypeCount, const std::shared_ptr<vertexBuffer>& layoutBuffer, u32 stride, u32 type, bool normalized) {
        this->bind();
        layoutBuffer->bind();
        u32 _stride = (stride != 0) ? stride : mVertexStride;
        u32 finalStride = 0;
        u32 finalOffset = 0;

        if (type == renderDataTypes::byte8 || type == renderDataTypes::unsignedByte8) {
            finalStride = _stride * sizeof(i8);
            finalOffset = mAttribOffset * sizeof(i8);
        } else if (type == renderDataTypes::short16 || type == renderDataTypes::unsignedShort16) {
            finalStride = _stride * sizeof(i16);
            finalOffset = mAttribOffset * sizeof(i16);
        } else if (type == renderDataTypes::int32 || type == renderDataTypes::unsignedInt32 || type == renderDataTypes::float32) {
            finalStride = _stride * sizeof(i32);
            finalOffset = mAttribOffset * sizeof(i32);
        } else {
            CALMAR_ASSERT_MSG(false, "Tried to set vertex layout attribute with unknown data type");
        }

        u32 attribElementCount = 0;
        switch (dataTypeCount) {
            case layoutAttributeType::FLOAT:
                attribElementCount = 1;
                break;
            case layoutAttributeType::INTEGER:
                attribElementCount = 1;
                break;
            case layoutAttributeType::VEC2:
                attribElementCount = 2;
                break;
            case layoutAttributeType::VEC3:
                attribElementCount = 3;
                break;
            case layoutAttributeType::VEC4:
                attribElementCount = 4;
                break;
            case layoutAttributeType::MAT2:
                attribElementCount = 8;
                break;
            case layoutAttributeType::MAT3:
                attribElementCount = 12;
                break;
            case layoutAttributeType::MAT4:
                attribElementCount = 16;
                break;
            default:
                break;
        }

        void* finalOffsetGl = (void*)(uintptr_t)(finalOffset);
        glVertexAttribPointer(mAttribIndex, attribElementCount, type, normalized, finalStride, finalOffsetGl);
        glEnableVertexAttribArray(mAttribIndex++);
        mAttribOffset += attribElementCount;
    }

}  // namespace calmar
