#pragma once

namespace calmar {
    class entityCamera {
       public:
        enum class projectionType { Perspective = 0,
                                    Orthographic = 1 };
        entityCamera();
        ~entityCamera() {}
        const glm::mat4& getProjection() const { return mProj; }

        void setPerspective(float verticalFOV, float nearClip, float farClip);
        void setOrthographic(float size, float nearClip, float farClip);

        void setViewportSize(u32 width, u32 height);

        float getPerspectiveVerticalFOV() const { return mPerspectiveFOV; }
        void setPerspectiveVerticalFOV(float verticalFov) {
            mPerspectiveFOV = verticalFov;
            recalculateProjection();
        }
        float getPerspectiveNearClip() const { return mPerspectiveNear; }
        void setPerspectiveNearClip(float nearClip) {
            mPerspectiveNear = nearClip;
            recalculateProjection();
        }
        float getPerspectiveFarClip() const { return mPerspectiveFar; }
        void setPerspectiveFarClip(float farClip) {
            mPerspectiveFar = farClip;
            recalculateProjection();
        }

        float getOrthographicSize() const { return mOrthographicSize; }
        void setOrthographicSize(float size) {
            mOrthographicSize = size;
            recalculateProjection();
        }
        float getOrthographicNearClip() const { return mOrthographicNear; }
        void setOrthographicNearClip(float nearClip) {
            mOrthographicNear = nearClip;
            recalculateProjection();
        }
        float getOrthographicFarClip() const { return mOrthographicFar; }
        void setOrthographicFarClip(float farClip) {
            mOrthographicFar = farClip;
            recalculateProjection();
        }

        projectionType getProjectionType() const { return mProjectionType; }
        void setProjectionType(projectionType type) {
            mProjectionType = type;
            recalculateProjection();
        }

        float getAspectRatio() const { return mAspectRatio; }

       private:
        void recalculateProjection();
        projectionType mProjectionType = projectionType::Orthographic;

        float mPerspectiveFOV = glm::radians(45.0f);
        float mPerspectiveNear = 0.01f, mPerspectiveFar = 1000.0f;

        float mOrthographicSize = 10.0f;
        float mOrthographicNear = -1.0f, mOrthographicFar = 1.0f;

        float mAspectRatio = 0.0f;

        glm::mat4 mProj = glm::mat4(1.0f);
    };
}  // namespace calmar
