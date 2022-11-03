#include "pch.h"
#include "gl_render_buffers.hpp"

#include <glad/glad.h>

namespace calmar {

    glVertexBuffer::glVertexBuffer(float* data, u32 size, u32 bufferFlags) {
        glCreateBuffers(1, &mId);
        glBindBuffer(GL_ARRAY_BUFFER, mId);
        glBufferData(GL_ARRAY_BUFFER, size, data, bufferFlags);
    }

    glVertexBuffer::glVertexBuffer(u32 size, u32 bufferFlags) {
        glCreateBuffers(1, &mId);
        glBindBuffer(GL_ARRAY_BUFFER, mId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, bufferFlags);
    }

    glVertexBuffer::~glVertexBuffer() {
        glDeleteBuffers(1, &mId);
    }
    void glVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, mId);
    }

    void glVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void glVertexBuffer::deleteId() {
        glDeleteBuffers(1, &mId);
    }

    void glVertexBuffer::setData(const void* data, u32 size, u32 offset) {
        glBindBuffer(GL_ARRAY_BUFFER, mId);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    glIndexBuffer::glIndexBuffer(u32* data, u32 count, u32 bufferFlags) {
        mCount = count;

        glCreateBuffers(1, &mId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, bufferFlags);
    }

    glIndexBuffer::~glIndexBuffer() {
        glDeleteBuffers(1, &mId);
    }

    void glIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
    }

    void glIndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void glIndexBuffer::deleteId() {
        glDeleteBuffers(1, &mId);
    }

}  // namespace calmar
