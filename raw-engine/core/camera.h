#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace core {
    // A fly camera: a position plus a look direction (yaw and pitch angles).
    // It only produces the view and projection matrices. Reading the keyboard and mouse is main's job.
    class Camera {

    public:
        // fovDegrees = vertical field of view. Starts looking down -Z.
        Camera(glm::vec3 position, float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        // Call when the window size changes, otherwise the picture gets stretched
        void setAspectRatio(float newAspectRatio);

        // Moves along the look direction / sideways / straight up. Negative = the other way.
        void moveForward(float amount);
        void moveRight(float amount);
        void moveUp(float amount);
        // Turns by these many degrees. Pitch is clamped so we can't flip upside down.
        void rotate(float yawDelta, float pitchDelta);

    private:
        glm::vec3 position;
        glm::vec3 front = {0.0f, 0.0f, -1.0f};   // where we look (always length 1)
        glm::vec3 right = {1.0f, 0.0f, 0.0f};    // our right-hand side
        glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
        float yaw = -90.0f;     // degrees, left/right. -90 = looking down -Z
        float pitch = 0.0f;     // degrees, up/down
        float fov, aspectRatio, nearPlane, farPlane;

        // yaw/pitch -> front and right. Call after every change to yaw or pitch.
        void updateVectors();
    };
}