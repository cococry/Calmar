#pragma once

#include "calmar/renderer/texture.hpp"

namespace calmar {
    class glTexture2d : public texture2d {
       public:
        glTexture2d(const std::string& filepath);
        ~glTexture2d();

        virtual void bind() override;

        virtual void activateSlot(u32 slot = 0) override;

        virtual void unbind() override;

        virtual void deleteId() override;

       private:
    };
}  // namespace calmar
