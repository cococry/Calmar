#include "orbit_camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "calmar/input/input.hpp"
#include "calmar/input/key_codes.hpp"
#include "calmar/input/mouse_codes.hpp"

#include "calmar/core/application.hpp"

#include "calmar/event_system/mouse_events.hpp"

#include "calmar/core/logging.hpp"
#include "calmar/core/application.hpp"

#include <algorithm>

namespace calmar {
    orbitCamera::orbitCamera(const cameraProperties& props) {
        mProps = props;

        updateView();
        mData.projMatrix = (glm::perspective(glm::radians(props.fov), props.aspectRatio, props.nearClip, props.farClip));
    }

    void orbitCamera::update() {
        if (input::isKeyDown(key::LeftAlt)) {
            glm::vec2 mousePos = {input::getMouseX(), input::getMouseY()};
            glm::vec2 mouseDelta = (mousePos - mData.initialMousePos) * 0.003f;
            mData.initialMousePos = mousePos;

            if (input::isMouseButtonDown(button::Middle)) {
                mousePan(mouseDelta);
            }

            if (input::isMouseButtonDown(button::Right)) {
                mouseZoom(mouseDelta.y);
            }

            if (input::isMouseButtonDown(button::Left)) {
                mouseRotate(mouseDelta);
            }
        }

        updateView();
    }

    void orbitCamera::resize(float width, float height) {
        mData.viewportWidth = width;
        mData.viewportHeight = height;
        updateProjection();
    }

    glm::vec3 orbitCamera::getUpDir() const {
        return glm::rotate(getLookDir(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 orbitCamera::getRightDir() const {
        return glm::rotate(getLookDir(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 orbitCamera::getForwardDir() const {
        return glm::rotate(getLookDir(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat orbitCamera::getLookDir() const {
        return glm::quat(glm::vec3(-mData.pitch, -mData.yaw, 0.0f));
    }

    void orbitCamera::updateProjection() {
        mProps.aspectRatio = mData.viewportWidth / mData.viewportHeight;
        mData.projMatrix = glm::perspective(glm::radians(mProps.fov), mProps.aspectRatio, mProps.nearClip, mProps.farClip);
    }

    void orbitCamera::updateView() {
        mData.position = calculatePosition();

        glm::quat dir = getLookDir();
        mData.viewMatrix = glm::translate(glm::mat4(1.0f), mData.position) * glm::toMat4(dir);
        mData.viewMatrix = glm::inverse(mData.viewMatrix);
    }

    void orbitCamera::handleEvents(event ev) {
        if (ev.type() == mouseScrolledEvent::evType) {
            const mouseScrolledEvent& mouseEv = static_cast<const mouseScrolledEvent&>(ev);
            float delta = mouseEv.getYOffset() * 0.1f;
            mouseZoom(delta);
            updateView();
        }
    }

    void orbitCamera::mousePan(const glm::vec2& mouseDelta) {
        glm::vec2 panspeed = getPanSpeed();
        mData.focalPoint += -getRightDir() * mouseDelta.x * panspeed.x * mData.oribtDistance;
        mData.focalPoint += getUpDir() * mouseDelta.y * panspeed.y * mData.oribtDistance;
    }

    void orbitCamera::mouseZoom(float delta) {
        mData.oribtDistance -= delta * getZoomSpeed();

        if (mData.oribtDistance < 0.5f) {
            mData.focalPoint += getForwardDir();
            mData.oribtDistance = 0.5f;
        }
    }

    void orbitCamera::mouseRotate(const glm::vec2& mouseDelta) {
        float yawSign = getUpDir().y < 0 ? -1.0f : 1.0f;
        mData.yaw += yawSign * mouseDelta.x * 1.36f;
        mData.pitch += mouseDelta.y * 1.36f;
    }

    glm::vec3 orbitCamera::calculatePosition() const {
        return mData.focalPoint - getForwardDir() * mData.oribtDistance;
    }

    glm::vec2 orbitCamera::getPanSpeed() const {
        float x = (mData.viewportWidth / 1000.0f) < (2.4f) ? (mData.viewportWidth / 1000.0f) : 2.4f;
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = (mData.viewportHeight / 1000.0f) < (2.4f) ? (mData.viewportHeight / 1000.0f) : 2.4f;
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float orbitCamera::getZoomSpeed() const {
        float distance = mData.oribtDistance * 0.2f;
        //((a) > (b)) ? (a) : (b)
        distance = distance > 0.0f ? distance : 0.0f;
        float speed = distance * distance;
        speed = speed < 100.0f ? speed : 100.0f;
        return speed;
    }
}  // namespace calmar