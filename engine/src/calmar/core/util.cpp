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
}  // namespace calmar
