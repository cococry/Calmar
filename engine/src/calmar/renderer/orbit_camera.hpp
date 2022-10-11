#pragma once

#include "calmar/core/defines.hpp"

#include "calmar/event_system/event.hpp"

#include <glm/glm.hpp>

namespace calmar {
    struct cameraProperties {
        cameraProperties(float fov = 45.0f, float nearClip = 0.1f, float farClip = 1000.0f, float aspectRatio = 1.779f)
            : fov(fov), nearClip(nearClip), farClip(farClip), aspectRatio(aspectRatio) {}

        float fov = 45.0f;
        float nearClip = 0.1f;
        float farClip = 1000.0f;
        float aspectRatio = 1.778f;
    };

    struct cameraData {
        glm::mat4 viewMatrix, projMatrix;
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 focalPoint = glm::vec3(0.0f);

        float oribtDistance = 10.0f;
        float pitch = 0.0f, yaw = 0.0f;

        float viewportWidth = 1280, viewportHeight = 720;

        float rotatationSpeed = 0.8f;

        glm::vec2 initialMousePos = glm::vec2(0.0f);
    };

    class orbitCamera {
       public:
        orbitCamera() = default;
        orbitCamera(const cameraProperties& props);

        void update();

        void resize(float width, float height);

        void handleEvents(const event& ev);

        glm::vec3 getUpDir() const;

        glm::vec3 getRightDir() const;

        glm::vec3 getForwardDir() const;

        glm::quat getLookDir() const;

        inline const cameraProperties& getProperties() const {
            return mProps;
        }

        inline const cameraData& getData() const {
            return mData;
        }

        inline glm::mat4 getViewProjection() const {
            return mData.projMatrix * mData.viewMatrix;
        }

       private:
        void updateProjection();

        void updateView();

        void mousePan(const glm::vec2& mouseDelta);

        void mouseZoom(float delta);

        glm::vec3 calculatePosition() const;

        glm::vec2 getPanSpeed() const;

        float getZoomSpeed() const;

        cameraProperties mProps;
        cameraData mData;
    };
}  // namespace calmar
