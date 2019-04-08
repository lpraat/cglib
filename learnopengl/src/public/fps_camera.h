#pragma once

#include "vec3.h"
#include "mat4.h"
#include "transform.h"
#include "quat.h"

namespace glp {

enum class FPSCameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

template <typename T>
class FPSCamera {
private:

    // TODO add this to constructor/config file
    const float32 speed = 5.0f;
    const float32 mouseSensitivity = 0.1f;
    const bool constrained = true;

    const Vec3<T> up;
    Quat<T> front;

    Vec3<T> position;
    Quat<T> orientation;

    T yaw;
    T pitch;

public:
    FPSCamera(Vec3<T> up = {0.0f, 1.0f, 0.0f}, Vec3<T> pos = {0.0f, 0.0f, 0.0f}) :
              up(up), front{0.0f, 0.0f, 0.0f, 1.0f}, position{0.0f, 0.0f, 3.0f}, orientation{front} {
        updateOrientation(0, 0);
    }

    Mat4<T> getView() {
        std::cout << yaw << " " << pitch << std::endl;
        return orientation.conjugate().toRotMatrix().dot(translate(-position));
    }

    void updatePosition(FPSCameraMovement cameraMovement, float32 deltaTime) {
        float32 delta = deltaTime * speed;

        Quat<T> frontQuaternion = orientation * front * orientation.conjugate();
        Vec3<T> front {frontQuaternion.x, frontQuaternion.y, frontQuaternion.z};
        Vec3<T> right {up.cross(front).normalize()};

        switch (cameraMovement)
        {
            case FPSCameraMovement::FORWARD:
                position -= delta * front;
                break;

            case FPSCameraMovement::BACKWARD:
                position += delta * front;
                break;

            case FPSCameraMovement::LEFT:
                position -= delta * right;
                break;

            case FPSCameraMovement::RIGHT:
                position += delta * right;
                break;

            default:
                break;
        }
    }

    void updateOrientation(float64 xOffset, float64 yOffset) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrained) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        Quat<T> qYaw = {-yaw, {0.0, 1.0f, 0.0f}};
        Quat<T> qPitch = {pitch, {1.0f, 0.0f, 0.0f}};

        orientation = qYaw * qPitch;
    }

};

}; // namespace glp