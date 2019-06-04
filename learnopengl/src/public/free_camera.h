#pragma once

#include "vec3.h"
#include "mat4.h"
#include "transform.h"
#include "quat.h"

namespace glp {

enum class FreeCameraMovement {
    FORWARD, BACKWARD,
    LEFT, RIGHT,
    UP, DOWN,
    ROLL_P,ROLL_M,
    PITCH_P, PITCH_M,
    YAW_P, YAW_M
};

template <typename T>
class FreeCamera {
private:
    // TODO add this to constructor/config file
    const float32 positionSpeed = 5.0f;
    const float32 orientationSpeed = 50.0f;
    const bool constrained = true;

    Vec3<T> position;
    Quat<T> orientation;

    T deltaUp, deltaForward, deltaRight;
    T roll, pitch, yaw;

    bool positionNeedsUpdate = true;
    bool orientationNeedsUpdate = true;

public:
    FreeCamera() :
         orientation{Quat<T> {0.0f, Vec3<T> {0, 0, 0}}}, position{0, 0, 3} {
    }

    Mat4<T> getView() {
        updateOrientation();
        updatePosition();
        return orientation.conjugate().toRotMatrix().dot(translate(-position));
    }

    Vec3<T> getPosition() const {
        return position;
    }

    Vec3<T> getFrontDirection() const {
        const Quat d = orientation * Quat<T> {0, 0, 0, 1.0f} * orientation.conjugate();
        return {d.x, d.y, d.z};
    }

    void updateDeltaPosition(FreeCameraMovement cameraMovement, float32 deltaTime) {
        const float32 delta = deltaTime * positionSpeed;

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
        const float32 delta = deltaTime * orientationSpeed;

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
            const Quat<T> qRoll {roll, {0, 0, 1}};
            const Quat<T> qPitch {pitch, {1, 0, 0}};
            const Quat<T> qYaw {yaw, {0, 1, 0}};

            orientation *= (qYaw * qPitch * qRoll);
            orientation.normalize();

            roll = 0, pitch = 0, yaw = 0;
            orientationNeedsUpdate = false;
        }
    }

    void updatePosition() {
        if (positionNeedsUpdate) {
            const Quat<T> deltaPos = orientation * Quat<T> {0, deltaRight, deltaUp, deltaForward} * orientation.conjugate();
            position += {deltaPos.x, deltaPos.y, deltaPos.z};

            deltaForward = 0, deltaUp = 0, deltaRight = 0;
            positionNeedsUpdate = false;
        }
    }

};

}; // namespace glp