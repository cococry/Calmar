#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/renderer/shader.hpp"

#include <vector>

namespace calmar {
    class glShader : public shader {
       public:
        glShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
        ~glShader();

        virtual void bind() override;
        virtual void unbind() override;
        virtual void deleteId() override;

       private:
        render_id compileShader(unsigned int shaderType, const std::string& shaderSource);

        void linkProgram(const std::vector<render_id>& shaders);
    };
}  // namespace calmar
