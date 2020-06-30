#pragma once

#include <core_types.h>
#include <iostream>
#include <cmath>
#include "constants.h"
#include "vec3.h"
#include "mat4.h"

namespace cglib {

template <typename T = float32>
struct Quat {

    union {
        struct {
            T a, b, c, d;
        };

        struct {
            T w, x, y, z;
        };
    };

    Quat(T a, T b, T c, T d) : a(a), b(b), c(c), d(d) {}

    Quat(T theta, const Vec3<T>& normalizedAxis) {
        const T cosThetaHalved = std::cos((theta * toRadians<T>()) / 2);
        const T sinThetaHalved = std::sin((theta * toRadians<T>()) / 2);

        a = cosThetaHalved;
        b = sinThetaHalved * normalizedAxis.x;
        c = sinThetaHalved * normalizedAxis.y;
        d = sinThetaHalved * normalizedAxis.z;
    }

    T length() const {
        return std::sqrt(a*a + b*b + c*c + d*d);
    }

    Quat<T>& normalize() {
        const T l = length();
        a /= l; b /= l; c /= l; d/= l;
        return *this;
    }

    Quat<T> conjugate() const {
        return {a, -b, -c, -d};
    }

    Quat<T> operator*(T scalar) const {
        return {a * scalar, b * scalar, c * scalar, d * scalar};
    }

    friend Quat<T> operator*(T scalar, const Quat<T>& quat) {
        return quat * scalar;
    }

    Quat<T>& operator+=(const Quat<T>& other) const {
        a += other.a; b += other.b; c += other.c; d += other.d;
        return *this;
    }

    Quat<T> operator+(const Quat<T>& other) const {
        return {a + other.a, b + other.b, c + other.c, d + other.d};
    }

    Quat<T>& operator*=(const Quat<T>& other) {
        a = a * other.a - b * other.b - c * other.c - d * other.d;
        b = a * other.b + b * other.a + c * other.d - d * other.c;
        c = a * other.c + c * other.a + d * other.b - b * other.d;
        d = a * other.d + d * other.a + b * other.c - c * other.b;
        return *this;
    }

    Quat<T> operator*(const Quat<T>& other) const {
        return {
            a * other.a - b * other.b - c * other.c - d * other.d,
            a * other.b + b * other.a + c * other.d - d * other.c,
            a * other.c + c * other.a + d * other.b - b * other.d,
            a * other.d + d * other.a + b * other.c - c * other.b
        };
    }

    Mat4<T> toRotMatrix() const {
        return {
            {1 - 2*(c*c + d*d), 2*(b*c - a*d), 2*(b*d + a*c), 0},
            {2*(b*c + a*d), 1 - 2*(b*b + d*d), 2*(c*d - a*b), 0},
            {2*(b*d - a*c), 2*(c*d + a*b), 1 - 2*(b*b + c*c), 0},
            {0, 0, 0, 1}
        };
    }

    void print() const {
        printf("Quat(%f, %f, %f, %f)\n", a, b, c, d);
    }
};

}; // namespace cglib

