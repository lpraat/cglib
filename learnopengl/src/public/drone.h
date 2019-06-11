#pragma once

#include "vec3.h"
#include "mat4.h"
#include "transform.h"
#include "quat.h"

namespace glp {

enum class DroneMovement {
    FORWARD, BACKWARD,
    LEFT, RIGHT,
    UP, DOWN,
    ROLL_P,ROLL_M,
    PITCH_P, PITCH_M,
    YAW_P, YAW_M
};

template <typename T>
class Drone {
private:
    // TODO add this to constructor/config file
    const float32 positionSpeed = 50.0f;
    const float32 orientationSpeed = 50.0f;
    const bool constrained = true;

    Vec3<T> position;
    Quat<T> orientation;

    T deltaUp, deltaForward, deltaRight;
    T roll, pitch, yaw;

    bool positionNeedsUpdate = true;
    bool orientationNeedsUpdate = true;

public:
    Drone() :
         orientation{Quat<T> {0.0f, Vec3<T> {0, 0, 0}}}, position{0, 0, 3} {
    }

    Mat4<T> getModel() {
        return glp::translate(position).dot(orientation.toRotMatrix());
    }

    std::pair<Vec3<T>, Mat4<T>> getLookAt() {
        Quat<float32> backQuat = orientation * Quat<float32> {0, 0, 0, 1} * orientation.conjugate();
        Vec3<float32> back = {backQuat.x, backQuat.y, backQuat.z};

        Quat<float32> upQuat = orientation * Quat<float32> {0, 0, 1, 0} * orientation.conjugate();
        Vec3<float32> up = {upQuat.x, upQuat.y, upQuat.z};

        Vec3<float32> lookAtPosition = position +20*back + 4*up;

        return std::make_pair(lookAtPosition, lookAt(lookAtPosition, position, up));
    }

    void setPosition(Vec3<T> position) {
        this->position = position;
    }

    void setOrientation(Quat<T> orientation) {
        this->orientation = orientation;
    }

    Vec3<T>& getPosition() {
        return position;
    }
    Quat<T>& getOrientation() {
        return orientation;
    }

    Vec3<T> getLightDirection() const {
        const glp::Quat<float32> d = orientation * glp::Quat<float32> {0, 0, 0.1f, 1.0f}.normalize() * orientation.conjugate();
        return {d.x, d.y, d.z};
    }

    void updateDeltaPosition(DroneMovement droneMovement, float32 deltaTime) {
        const float32 delta = deltaTime * positionSpeed;

        switch (droneMovement)
        {
            case DroneMovement::FORWARD:
                deltaForward -= delta;
                break;
            case DroneMovement::BACKWARD:
                deltaForward += delta;
                break;
            case DroneMovement::LEFT:
                deltaRight -= delta;
                break;

            case DroneMovement::RIGHT:
                deltaRight += delta;
                break;

            case DroneMovement::UP:
                deltaUp += delta;
                break;

            case DroneMovement::DOWN:
                deltaUp -= delta;
                break;

            default:
                break;
        }

        positionNeedsUpdate = true;
    }

    void updateDeltaOrientation(DroneMovement droneMovement, float32 deltaTime) {
        const float32 delta = deltaTime * orientationSpeed;

        switch (droneMovement)
        {
            case DroneMovement::ROLL_P:
                roll += delta;
                break;

            case DroneMovement::ROLL_M:
                roll -= delta;
                break;

            case DroneMovement::PITCH_P:
                pitch += delta;
                break;

            case DroneMovement::PITCH_M:
                pitch -= delta;
                break;

            case DroneMovement::YAW_P:
                yaw += delta;
                break;

            case DroneMovement::YAW_M:
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