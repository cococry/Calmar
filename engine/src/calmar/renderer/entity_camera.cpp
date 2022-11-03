#include "pch.h"
#include "entity_camera.hpp"
#include "calmar/core/application.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace calmar {
    entityCamera::entityCamera() {
        recalculateProjection();
        setViewportSize(application::getInstance()->display->getProperties().width, application::getInstance()->display->getProperties().height);
    }
    void entityCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
        mProjectionType = projectionType::Perspective;
        mPerspectiveFOV = verticalFOV;
        mPerspectiveNear = nearClip;
        mPerspectiveFar = farClip;
        recalculateProjection();
    }
    void entityCamera::setOrthographic(float size, float nearClip, float farClip) {
        mProjectionType = projectionType::Orthographic;
        mOrthographicSize = size;
        mOrthographicNear = nearClip;
        mOrthographicFar = farClip;
        recalculateProjection();
    }
    void entityCamera::recalculateProjection() {
        if (mProjectionType == projectionType::Perspective) {
            mProj = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
        } else {
            float left = -mOrthographicSize * mAspectRatio * 0.5f;
            float right = mOrthographicSize * mAspectRatio * 0.5f;
            float bottom = -mOrthographicSize * 0.5f;
            float top = mOrthographicSize * 0.5f;

            mProj = glm::ortho(left, right, bottom, top, mOrthographicNear, mOrthographicFar);
        }
    }
    void entityCamera::setViewportSize(u32 width, u32 height) {
        mAspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

}  // namespace calmar
