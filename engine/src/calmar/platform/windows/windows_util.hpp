#pragma once

namespace calmar {
    namespace platform {
        class windowsFileDialogs {
           public:
            static std::string openFile(const char* filter, const char* dir);
            
            static std::string saveFile(const char* filter);

           private:
        };
    }  // namespace platform

}  // namespace calmar
