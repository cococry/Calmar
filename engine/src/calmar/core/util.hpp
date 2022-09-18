#pragma once

#include <string>

namespace calmar {
    class util {
       public:
        static std::string getFileContents(const std::string& filepath);
    };
}  // namespace calmar
