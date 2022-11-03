#pragma once

#include "calmar/renderer/shader.hpp"

namespace calmar {
    class glShader : public shader {
       public:
        glShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
        ~glShader();

        virtual void bind() override;
        virtual void unbind() override;
        virtual void deleteId() override;

        virtual void setMatrix4f(const std::string& name, const glm::mat4& value) override;
        virtual void setMatrix3f(const std::string& name, const glm::mat3& value) override;
        virtual void setMatrix2f(const std::string& name, const glm::mat2& value) override;

        virtual void setVector4f(const std::string& name, const glm::vec4& value) override;
        virtual void setVector3f(const std::string& name, const glm::vec3& value) override;
        virtual void setVector2f(const std::string& name, const glm::vec2& value) override;

        virtual void setInt(const std::string& name, int value) override;
        virtual void setFloat(const std::string& name, float value) override;

        void setIntArray(const std::string& name, i32* values, u32 count) override;

       private:
        render_id compileShader(unsigned int shaderType, const std::string& shaderSource);

        void linkProgram(const std::vector<render_id>& shaders);
    };
}  // namespace calmar
