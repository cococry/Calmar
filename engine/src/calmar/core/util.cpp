#include "util.hpp"

#include <fstream>
#include <sstream>

#include "calmar/core/logging.hpp"

#include "calmar/platform/windows/windows_util.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace calmar {
    namespace platform {
        std::string fileDialogs::openFile(const char* filter, const char* dir) {
#ifdef CALMAR_PLATFORM_WINDOWS
            return platform::windowsFileDialogs::openFile(filter, dir);
#else
            return std::string("");
#endif
        }
    }  // namespace platform
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
    namespace math {
        bool linalg::decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
            // From glm::decompose in matrix_decompose.inl

            using namespace glm;
            using T = float;

            mat4 LocalMatrix(transform);

            // Normalize the matrix.
            if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
                return false;

            // First, isolate perspective.  This is the messiest.
            if (
                epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
                epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
                epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
                // Clear the perspective partition
                LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
                LocalMatrix[3][3] = static_cast<T>(1);
            }

            // Next take care of translation (easy).
            translation = vec3(LocalMatrix[3]);
            LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

            vec3 Row[3];

            // Now get scale and shear.
            for (length_t i = 0; i < 3; ++i)
                for (length_t j = 0; j < 3; ++j)
                    Row[i][j] = LocalMatrix[i][j];

            // Compute X scale factor and normalize first row.
            scale.x = length(Row[0]);
            Row[0] = detail::scale(Row[0], static_cast<T>(1));
            scale.y = length(Row[1]);
            Row[1] = detail::scale(Row[1], static_cast<T>(1));
            scale.z = length(Row[2]);
            Row[2] = detail::scale(Row[2], static_cast<T>(1));

            // At this point, the matrix (in rows[]) is orthonormal.
            // Check for a coordinate system flip.  If the determinant
            // is -1, then negate the matrix and the scaling factors.

            rotation.y = asin(-Row[0][2]);
            if (cos(rotation.y) != 0) {
                rotation.x = atan2(Row[1][2], Row[2][2]);
                rotation.z = atan2(Row[0][1], Row[0][0]);
            } else {
                rotation.x = atan2(-Row[2][0], Row[1][1]);
                rotation.z = 0;
            }

            return true;
        }
    }  // namespace math
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
