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

    void glVertexArray::setVertexLayoutAttribute(u32 size, u32 stride, u32 type, bool normalized) {
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
            finalOffset = _stride * sizeof(i32);
        } else {
        }

        glVertexAttribPointer(mAttribIndex, size, type, normalized, finalStride, (void*)finalOffset);
        glEnableVertexAttribArray(mAttribIndex++);
        mAttribOffset += size;
    }

}  // namespace calmar
