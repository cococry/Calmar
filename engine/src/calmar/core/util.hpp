#pragma once

#include <string>

#include <vector>

#include <glm/glm.hpp>

namespace calmar {
    namespace platform {
        class fileDialogs {
           public:
            static std::string openFile(const char* filter, const char* dir = "../engine/assets");

           private:
        };
    }  // namespace platform
    namespace math {
        class linalg {
           public:
            static bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
        };
    }  // namespace math
    class util {
       public:
        static std::string getFileContents(const std::string& filepath);

        static std::vector<char> getFileContentsBinary(const std::string& filepath);
    };
}  // namespace calmar
