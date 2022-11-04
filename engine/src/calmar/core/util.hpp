#pragma once


namespace calmar {
    namespace platform {
        class fileDialogs {
           public:
            static std::string openFile(const char* filter, const char* dir = "../engine/assets");

            static std::string saveFile(const char* filter);
            
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

        static char* getFileContentsBytes(const std::string& filepath, u32* oSize);
    };
}  // namespace calmar
