#pragma once

#include <core_types.h>
#include <iostream>
#include <cmath>
#include "constants.h"
#include "vec3.h"

// TODO modify this like Mat4, Vec4 ecc..
namespace glp {

template <typename T = float32>
struct Quat {
    T a, b, c, d;

    Quat(T a, T b, T c, T d) : a(a), b(b), c(c), d(d) {}

    // TODO change with fixed vector of size 3
    Quat(T theta, Vec3<T> axis) {
        axis.normalize();
        T cosThetaHalved = std::cos((theta * toRadians<T>()) / 2);
        T sinThetaHalved = std::sin((theta * toRadians<T>()) / 2);

        a = cosThetaHalved;
        b = sinThetaHalved * axis.x;
        c = sinThetaHalved * axis.y;
        d = sinThetaHalved * axis.z;
    }

    // TODO Add +=
    Quat<T> operator+(const Quat<T>& other) const {
        return {a + other.a, b + other.b, c + other.c, d + other.d};
    }

    Quat<T> operator*(T scalar) const {
        return {a * scalar, b * scalar, c * scalar, d * scalar};
    }

    friend Quat<T> operator*(T scalar, const Quat<T>& quat) {
        return quat * scalar;
    }

    Quat<T> operator*(const Quat<T>& other) const {
        return {
            a * other.a - b * other.b - c * other.c - d * other.d,
            a * other.b + b * other.a + c * other.d - d * other.c,
            a * other.c + c * other.a + d * other.b - b * other.d,
            a * other.d + d * other.a + b * other.c - c * other.b
        };
    }

    Mat4<T> toRotMatrix() {
        return {
            {1 - 2*(c*c + d*d), 2*(b*c + a*d), 2*(b*d - a*c), 0},
            {2*(b*c - a*d), 1 - 2*(b*b + d*d), 2*(c*d + a*b), 0},
            {2*(b*d + a*c), 2*(c*d - a*b), 1 - 2*(b*b + c*c), 0},
            {0, 0, 0, 1}
        };
    }

    void print() const {
        printf("Quat(%f, %f, %f, %f)\n", a, b, c, d);
    }
};

}; // namespace glp

