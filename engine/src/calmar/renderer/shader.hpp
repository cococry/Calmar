#pragma once

namespace calmar {
    class shader {
       public:
        shader() {}
        virtual ~shader() {}

        static shader create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

        static std::shared_ptr<shader> createRef(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

        static std::unique_ptr<shader> createScoped(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

        virtual void bind() {}

        virtual void unbind() {}

        virtual void deleteId() {}

        virtual void setMatrix4f(const std::string& name, const glm::mat4& value) {}
        virtual void setMatrix3f(const std::string& name, const glm::mat3& value) {}
        virtual void setMatrix2f(const std::string& name, const glm::mat2& value) {}

        virtual void setVector4f(const std::string& name, const glm::vec4& value) {}
        virtual void setVector3f(const std::string& name, const glm::vec3& value) {}
        virtual void setVector2f(const std::string& name, const glm::vec2& value) {}

        virtual void setInt(const std::string& name, int value) {}
        virtual void setFloat(const std::string& name, float value) {}

        virtual void setIntArray(const std::string& name, i32* values, u32 count) {}

        inline render_id getId() const {
            return mId;
        }

       protected:
        render_id mId;
    };
}  // namespace calmar
