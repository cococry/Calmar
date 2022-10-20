#pragma once

#include "calmar/core/defines.hpp"

#include <calmar/ecs/scene.hpp>
#include <memory>

using namespace calmar;

namespace calmarEd {
    class sceneManager {
       public:
        sceneManager();

        sceneManager(const std::shared_ptr<scene>& editorScene);

        void updateActiveScene();

        void startRuntimeScene();

        void stopRuntimeScene();

        inline const std::shared_ptr<scene>& getActiveScene() const {
            return mActiveScene;
        }

        inline const std::shared_ptr<scene>& getEdiorScene() const {
            return mEditorScene;
        }

        inline bool inEditor() const {
            return mInEditor;
        }

       private:
        std::shared_ptr<scene> mEditorScene;
        std::shared_ptr<scene> mActiveScene;
        bool mInEditor = true;
    };
}  // namespace calmarEd
