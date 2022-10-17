#include "gl_texture.hpp"

#include <stb_image.h>

#include "calmar/core/logging.hpp"
#include "calmar/core/asserting.hpp"
#include "calmar/core/asset_pool.hpp"

#include <glad/glad.h>

namespace calmar {
    glTexture2d::glTexture2d(const std::string& filepath) {
        mTextureData.filepath = filepath;
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = nullptr;

        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        CALMAR_ASSERT_MSG(data, "Failed to load texture with stb_image/OpenGL at location '{0}'.", filepath);

        mTextureData.width = width;
        mTextureData.height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        mTextureData.internalFormat = internalFormat;
        mTextureData.dataFormat = dataFormat;

        CALMAR_ASSERT_MSG(internalFormat & dataFormat, "Texture at location '{0}' uses a not supported data format.", filepath);

        glCreateTextures(GL_TEXTURE_2D, 1, &mId);
        glTextureStorage2D(mId, 1, internalFormat, mTextureData.width, mTextureData.height);

        glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(mId, 0, 0, 0, mTextureData.width, mTextureData.height, dataFormat, GL_UNSIGNED_BYTE, data);

        CALMAR_TRACE("Successsfully loaded texture at filepath'{0}' (channels: {1} size: {2}x{3}) with stb_image/OpenGL.", filepath, channels, mTextureData.width, mTextureData.height);

        stbi_image_free(data);
    }

    glTexture2d::glTexture2d(u32 width, u32 height) {
        mTextureData.width = width;
        mTextureData.height = height;

        mTextureData.internalFormat = GL_RGBA8;
        mTextureData.dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &mId);
        glTextureStorage2D(mId, 1, mTextureData.internalFormat, mTextureData.width, mTextureData.height);

        glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        CALMAR_TRACE("Created texture object with width, height: {0}x{1} successfully.", width, height);
    }
    glTexture2d::~glTexture2d() {
        glDeleteTextures(1, &mId);
    }
    void glTexture2d::bind() {
        glBindTexture(GL_TEXTURE_2D, mId);
    }
    void glTexture2d::activateSlot(u32 slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
    }
    void glTexture2d::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void glTexture2d::deleteId() {
        glDeleteTextures(1, &mId);
    }

    void glTexture2d::setData(void* data, u32 size) {
        u32 bytesPerPixel = mTextureData.dataFormat == GL_RGBA ? 4 : 3;
        CALMAR_ASSERT_MSG(mTextureData.width * mTextureData.height * bytesPerPixel == size, "Can only set texture data for the entire texture.");

        glTextureSubImage2D(mId, 0, 0, 0, mTextureData.width, mTextureData.height, mTextureData.dataFormat, GL_UNSIGNED_BYTE, data);
    }
    bool glTexture2d::operator==(const texture2d& other) const {
        return mId == other.getId();
    }

    void glTexture2d::bindUnit(u32 slot) {
        glBindTextureUnit(slot, mId);
    }
}  // namespace calmar
