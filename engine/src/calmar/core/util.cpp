#include "util.hpp"

#include <fstream>
#include <sstream>

#include "calmar/core/logging.hpp"

namespace calmar {
    std::string util::getFileContents(const std::string& filepath) {
        std::string source = "";
        std::stringstream stringBuf;
        std::ifstream file;

        file.open(filepath);

        if (file.is_open()) {
            stringBuf << file.rdbuf();
            source = stringBuf.str();
        } else {
            CALMAR_ERROR("Failed to open & read file at location '{0}'.", filepath);
        }

        file.close();

        return source;
    }

    std::vector<char> util::getFileContentsBinary(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            CALMAR_ERROR("Failed to open & read file at location '{0}'.", filepath);
        }

        u64 fileSize = (u64)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
}  // namespace calmar
