#include "pch.h"
#include "gl_frame_buffer.hpp"


#include <glad/glad.h>

#define MAX_FRAMEBUFFER_SIZE 8192

namespace calmar {
    namespace util {
        static GLenum textureTarget(bool multisampled) {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }
        static void attachFramebufferColorTexture(u32 id, i32 samples, GLenum glFormat, GLenum format, u32 width, u32 height, i32 index) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, glFormat, width, height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
        }

        static void attachFramebufferDepthTexture(u32 id, i32 samples, GLenum format, GLenum attachmentType, u32 width, u32 height) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
        }
        static void createTextures(bool multisampled, u32* oId, u32 count) {
            glCreateTextures(textureTarget(multisampled), count, oId);
        }
        static void bindTexture(bool multisampled, u32 id) {
            glBindTexture(textureTarget(multisampled), id);
        }
        static bool isDepthFormat(framebufferTextureFormat format) {
            return format == calmar::framebufferTextureFormat::DEPTH24STENCIL8;
        }

    }  // namespace util
    glFrameBuffer::glFrameBuffer(const framebufferProperties& props)
        : mProps(props) {
        for (auto format : mProps.attachments.attachments) {
            if (!util::isDepthFormat(format.textureFormat))
                mColorAttachmentProps.emplace_back(format);
            else
                mDepthAttachmentProps = format;
        }
        reConstruct();
    }

    glFrameBuffer::~glFrameBuffer() {
        glDeleteFramebuffers(1, &mId);
        glDeleteTextures(static_cast<GLsizei>(mColorAttachments.size()), mColorAttachments.data());
        glDeleteTextures(1, &mDepthAttachment);
    }

    void glFrameBuffer::reConstruct() {
        if (mId) {
            glDeleteFramebuffers(1, &mId);
            glDeleteTextures(static_cast<GLsizei>(mColorAttachments.size()), mColorAttachments.data());
            glDeleteTextures(1, &mDepthAttachment);

            mColorAttachments.clear();
            mDepthAttachment = 0;
        }
        glCreateFramebuffers(1, &mId);
        glBindFramebuffer(GL_FRAMEBUFFER, mId);

        bool multisampled = mProps.samples > 1;
        if (mColorAttachmentProps.size()) {
            mColorAttachments.resize(mColorAttachmentProps.size());
            util::createTextures(multisampled, mColorAttachments.data(), (u32)mColorAttachments.size());

            for (i32 i = 0; i < mColorAttachments.size(); i++) {
                util::bindTexture(multisampled, mColorAttachments[i]);
                switch (mColorAttachmentProps[i].textureFormat) {
                    case framebufferTextureFormat::RGBA8:
                        util::attachFramebufferColorTexture(mColorAttachments[i], mProps.samples, GL_RGBA8, GL_RGBA, mProps.width, mProps.height, i);
                        break;
                    case framebufferTextureFormat::SHADER_RED_INT:
                        util::attachFramebufferColorTexture(mColorAttachments[i], mProps.samples, GL_R32I, GL_RED_INTEGER, mProps.width, mProps.height, i);
                        break;
                    default:
                        break;
                }
            }
        }
        if (mDepthAttachmentProps.textureFormat != framebufferTextureFormat::NONE) {
            util::createTextures(multisampled, &mDepthAttachment, 1);
            util::bindTexture(multisampled, mDepthAttachment);
            switch (mDepthAttachmentProps.textureFormat) {
                case framebufferTextureFormat::DEPTH24STENCIL8:
                    util::attachFramebufferDepthTexture(mDepthAttachment, mProps.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, mProps.width, mProps.height);
                    break;
                default:
                    break;
            }
        }
        if (mColorAttachments.size() > 1) {
            CALMAR_ASSERT_MSG(mColorAttachments.size() <= 4, "Tried to use more than 4 color attachments in framebuffer");
            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers((u32)mColorAttachments.size(), buffers);
        } else if (mColorAttachments.empty()) {
            glDrawBuffer(GL_NONE);
        }

        CALMAR_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void glFrameBuffer::resize(u32 width, u32 height) {
        mProps.width = width;
        mProps.height = height;
        reConstruct();
    }

    void glFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, mId);
        glViewport(0, 0, mProps.width, mProps.height);
    }
    void glFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void glFrameBuffer::clearAttachment(u32 attachmentIndex, i32 data) {
        CALMAR_ASSERT_MSG(attachmentIndex < mColorAttachmentProps.size(), "Attachment index out of bounds.");

        glClearTexImage(mColorAttachments[attachmentIndex], 0, GL_RED_INTEGER, GL_INT, &data);
    }

    i32 glFrameBuffer::readPixel(u32 attachmentIndex, i32 x, i32 y) {
        CALMAR_ASSERT_MSG(attachmentIndex < mColorAttachmentProps.size(), "Attachment index out of bounds.");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }
}  // namespace calmar
