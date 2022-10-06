#pragma once

#include <string>

#include <vector>

namespace calmar {
    class util {
       public:
        static std::string getFileContents(const std::string& filepath);

        static std::vector<char> getFileContentsBinary(const std::string& filepath);
    };
}  // namespace calmar
