#pragma once

#include "vec3.h"
#include "mat4.h"
#include "transform.h"
#include "quat.h"

namespace glp {

enum class FreeCameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROLL_P,
    ROLL_M,
    PITCH_P,
    PITCH_M,
    YAW_P,
    YAW_M
};

template <typename T>
class FreeCamera {
private:
    // TODO add this to constructor/config file
    const float32 positionSpeed = 5.0f;
    const float32 orientationSpeed = 50.0f;
    const float32 mouseSensitivity = 0.1f;
    const bool constrained = true;

    Vec3<T> position;
    Quat<T> orientation;

    T deltaRight;
    T deltaForward;
    T deltaUp;

    T roll;
    T pitch;
    T yaw;

    bool positionNeedsUpdate = true;
    bool orientationNeedsUpdate = true;

public:
    FreeCamera() :
         orientation{Quat {-360.0f, Vec3 {0.0f, 0.0f, 1.0f}}}, position{0.0f, 0.0f, 3.0f} {
    }

    Mat4<T> getView() {
        updateOrientation();
        updatePosition();
        return orientation.conjugate().toRotMatrix().dot(translate(-position));
    }

    void updateDeltaPosition(FreeCameraMovement cameraMovement, float32 deltaTime) {
        float32 delta = deltaTime * positionSpeed;

        switch (cameraMovement)
        {
            case FreeCameraMovement::FORWARD:
                deltaForward -= delta;
                break;
            case FreeCameraMovement::BACKWARD:
                deltaForward += delta;
                break;
            case FreeCameraMovement::LEFT:
                deltaRight -= delta;
                break;

            case FreeCameraMovement::RIGHT:
                deltaRight += delta;
                break;

            case FreeCameraMovement::UP:
                deltaUp += delta;
                break;

            case FreeCameraMovement::DOWN:
                deltaUp -= delta;
                break;

            default:
                break;
        }

        positionNeedsUpdate = true;
    }

    void updateDeltaOrientation(FreeCameraMovement cameraMovement, float32 deltaTime) {
        float32 delta = deltaTime * orientationSpeed;

        switch (cameraMovement)
        {
            case FreeCameraMovement::ROLL_P:
                roll += delta;
                break;

            case FreeCameraMovement::ROLL_M:
                roll -= delta;
                break;

            case FreeCameraMovement::PITCH_P:
                pitch += delta;
                break;

            case FreeCameraMovement::PITCH_M:
                pitch -= delta;
                break;

            case FreeCameraMovement::YAW_P:
                yaw += delta;
                break;

            case FreeCameraMovement::YAW_M:
                yaw -= delta;
                break;

            default:
                break;
        }
        orientationNeedsUpdate = true;
    }

    void updateOrientation() {
        if (orientationNeedsUpdate) {
            Quat<T> qRoll = {roll, {0.0f, 0.0f, 1.0f}};
            Quat<T> qPitch = {pitch, {1.0f, 0.0f, 0.0f}};
            Quat<T> qYaw = {yaw, {0.0, 1.0f, 0.0f}};

            orientation = orientation * (qYaw* qPitch * qRoll);
            orientation.normalize();

            roll = 0;
            pitch = 0;
            yaw = 0;
            orientationNeedsUpdate = false;
        }
    }

    void updatePosition() {
        if (positionNeedsUpdate) {
            Quat<T> deltaPos = orientation * Quat<T> {0.0f, deltaRight, deltaUp, deltaForward} * orientation.conjugate();
            position += {deltaPos.x, deltaPos.y, deltaPos.z};

            deltaForward = 0;
            deltaUp = 0;
            deltaRight = 0;
            positionNeedsUpdate = false;
        }
    }

    // TODO put constraints on angles

    // void updateOrientation(float64 xOffset, float64 yOffset) {
    //     xOffset *= mouseSensitivity;
    //     yOffset *= mouseSensitivity;

    //     yaw += xOffset;
    //     pitch += yOffset;

    //     Quat<T> qYaw = {-yaw, {0.0, 1.0f, 0.0f}};
    //     Quat<T> qPitch = {pitch, {1.0f, 0.0f, 0.0f}};

    //     orientation = qYaw * qPitch;
    // }

};

}; // namespace glp