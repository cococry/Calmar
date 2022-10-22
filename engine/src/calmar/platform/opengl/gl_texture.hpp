#pragma once

#include "calmar/renderer/texture.hpp"

namespace calmar {
    class glTexture2d : public texture2d {
       public:
        glTexture2d(const std::string& filepath, textureFilterMode filterMode);
        glTexture2d(u32 width, u32 height);
        ~glTexture2d();

        virtual void bind() override;

        virtual void bindUnit(u32 slot) override;

        virtual void activateSlot(u32 slot = 0) override;

        virtual void unbind() override;

        virtual void deleteId() override;

        virtual void setData(void* data, u32 size) override;
        virtual bool operator==(const texture2d& other) const override;

       private:
    };
}  // namespace calmar
