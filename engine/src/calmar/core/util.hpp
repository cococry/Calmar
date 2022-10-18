#pragma once

#include <string>

#include <vector>

namespace calmar {
    namespace platform {
        class fileDialogs {
           public:
            static std::string openFile(const char* filter, const char* dir = "../engine/assets");

           private:
        };
    }  // namespace platform
    class util {
       public:
        static std::string getFileContents(const std::string& filepath);

        static std::vector<char> getFileContentsBinary(const std::string& filepath);
    };
}  // namespace calmar
