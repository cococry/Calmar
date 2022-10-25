#pragma once

#include "calmar/core/defines.hpp"

#include "scene.hpp"

#include <memory>

namespace calmar {
    class sceneSerialzer {
       public:
        sceneSerialzer(const std::shared_ptr<scene>& inScene);

        void serialize(const std::string& filepath);

        bool deserialize(const std::string& filepath);

       private:
        std::shared_ptr<scene> mScene;
    };
}  // namespace calmar
