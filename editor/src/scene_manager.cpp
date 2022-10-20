#include "scene_manager.hpp"

namespace calmarEd {
    sceneManager::sceneManager() {
    }
    sceneManager::sceneManager(const std::shared_ptr<scene>& editorScene) {
        mActiveScene = editorScene;
        mEditorScene = mActiveScene;
    }
    void sceneManager::updateActiveScene() {
        if (mInEditor) {
            mActiveScene->update();
        }

        else {
            mActiveScene->updateRuntime();
        }
    }
    void sceneManager::startRuntimeScene() {
        mInEditor = false;
        mActiveScene = scene::copy(mEditorScene);
    }
    void sceneManager::stopRuntimeScene() {
        mInEditor = true;
        mActiveScene = mEditorScene;
    }
}  // namespace calmarEd
