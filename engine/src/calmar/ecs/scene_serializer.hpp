#pragma once

#include "scene.hpp"

namespace calmar {
    struct physicsSettings {
        i32 velocityIterations, positionIterations;
        float gravityScale;
    };
    class sceneSerialzer {
       public:
        sceneSerialzer(const std::shared_ptr<scene>& inScene);

        void serialize(const std::string& filepath);

        bool deserialize(const std::string& filepath);

        physicsSettings deserialzePhysicsSettings(const std::string& filepath);

       private:
        std::shared_ptr<scene> mScene;
    };
}  // namespace calmar
