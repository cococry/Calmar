#pragma once

#include "scene.hpp"

namespace calmar {
    class sceneSerialzer {
       public:
        sceneSerialzer(const std::shared_ptr<scene>& inScene);

        void serialize(const std::string& filepath);

        bool deserialize(const std::string& filepath);

        glm::vec2 deserialzePhysicsSettings(const std::string& filepath);

       private:
        std::shared_ptr<scene> mScene;
    };
}  // namespace calmar
