#pragma once

#include "vec3.h"
#include "mat4.h"
#include "transform.h"
#include "quat.h"

namespace cglib {

enum class FPSCameraMovement {
    FORWARD, BACKWARD,
    LEFT, RIGHT
};

template <typename T>
class FPSCamera {
private:

    const float32 speed = 5.0f;
    const float32 mouseSensitivity = 0.1f;
    const bool constrained = true;

    const Vec3<T> up;
    Quat<T> front;

    Vec3<T> position;
    Quat<T> orientation;

    T pitch, yaw;
    T fov;

public:
    FPSCamera(T fov, Vec3<T> up = {0, 1, 0}, Vec3<T> pos = {0, 0, 0}) :
              up(up), front{0, 0, 0, 1}, position{0, 0, 3}, orientation{front}, fov(fov) {
        updateOrientation(0, 0);
    }

    Mat4<T> getView() {
        return orientation.conjugate().toRotMatrix().dot(translate(-position));
    }

    T getFov() {
        return fov;
    }

    void updatePosition(FPSCameraMovement cameraMovement, float32 deltaTime) {
        const float32 delta = deltaTime * speed;

        const Quat<T> frontQuaternion = orientation * front * orientation.conjugate();

        // To have a true fps camera()
        // Vec3<T> front {frontQuaternion.x, 0, frontQuaterion.z};

        const Vec3<T> front {frontQuaternion.x, frontQuaternion.y, frontQuaternion.z};
        const Vec3<T> right {up.cross(front).normalize()};

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
            if (pitch > 89) {
                pitch = 89;
            }
            if (pitch < -89) {
                pitch = -89;
            }
        }

        const Quat<T> qYaw {-yaw, {0, 1, 0}};
        const Quat<T> qPitch {pitch, {1, 0, 0}};

        orientation = qYaw * qPitch;
    }

    void updateZoom(float64 yOffset) {
        if(fov >= 1 && fov <= 45)
            fov -= yOffset;
        if(fov <= 1)
            fov = 1;
        if(fov >= 45)
            fov = 45;
    }

};

}; // namespace cglib