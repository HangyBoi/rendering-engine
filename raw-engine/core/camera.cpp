#include "camera.h"

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {
    Camera::Camera(glm::vec3 position, float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
        : position(position), fov(fovDegrees), aspectRatio(aspectRatio), nearPlane(nearPlane),farPlane(farPlane) {
        updateVectors();
    }

    // View matrix: "stand at position, look at the point one step ahead"
    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, worldUp);
    }

    // Projection matrix: the lens (field of view, window shape, nearest and farthest visible distance)
    glm::mat4 Camera::getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void Camera::setAspectRatio(float newAspectRatio) {
        if (newAspectRatio <= 0.0f) return;  // e.g. a minimized window: keep the old value
        aspectRatio = newAspectRatio;
    }

    void Camera::moveForward(float amount) {
        position += front * amount;
    }

    void Camera::moveRight(float amount) {
        position += right * amount;
    }

    void Camera::moveUp(float amount) {
        position += worldUp * amount;
    }

    void Camera::rotate(float yawDelta, float pitchDelta) {
        yaw += yawDelta;
        // Straight up/down (90) would line "front" up with worldUp and lookAt would flip
        pitch = std::clamp(pitch + pitchDelta, -89.0f, 89.0f);
        updateVectors();
    }

    void Camera::updateVectors() {
        const float yawRad = glm::radians(yaw);
        const float pitchRad = glm::radians(pitch);

        front = glm::normalize(glm::vec3(std::cos(yawRad) * std::cos(pitchRad),
                                           std::sin(pitchRad),
                                           std::sin(yawRad) * std::cos(pitchRad)));
        // Sideways direction: perpendicular to both "front" and "up"
        right = glm::normalize(glm::cross(front, worldUp));
    }
}
