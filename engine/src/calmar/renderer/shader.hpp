#pragma once

#include "calmar/core/defines.hpp"

#include <memory>
#include <string>

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

        inline render_id getId() const {
            return mId;
        }

       protected:
        render_id mId;
    };
}  // namespace calmar
