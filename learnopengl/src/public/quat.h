#pragma once

#include <core_types.h>
#include <iostream>
#include <cmath>
#include "constants.h"

class Quat {
private:
    float32 a;
    float32 b;
    float32 c;
    float32 d;

public:
    Quat(float32 a, float32 b, float32 c, float32 d) : a(a), b(b), c(c), d(d) {}

    // TODO change with fixed vector of size 3
    Quat(float32 theta, std::vector<float32> axis) {
        float32 cosThetaHalved = std::cos((theta * pi()) / 180 / 2);
        float32 sinThetaHalved = std::sin((theta * pi()) / 180 / 2);

        a = cosThetaHalved;
        b = sinThetaHalved * axis[0];
        c = sinThetaHalved * axis[1];
        d = sinThetaHalved * axis[2];
    }

    Quat operator+(const Quat& other) const {
        return Quat {a + other.a, b + other.b, c + other.c, d + other.d};
    }

    Quat operator*(float32 scalar) const {
        return Quat {a * scalar, b * scalar, c * scalar, d * scalar};
    }

    friend Quat operator*(float32 scalar, const Quat& quat) {
        return quat * scalar;
    }

    Quat operator*(const Quat& other) const {
        return Quat {
            a * other.a - b * other.b - c * other.c - d * other.d,
            a * other.b + b * other.a + c * other.d - d * other.c,
            a * other.c + c * other.a + d * other.b - b * other.d,
            a * other.d + d * other.a + b * other.c - c * other.b
        };
    }

    // TODO change with fixed matrix of size 4
    matrix<float32> toRotMatrix() {
        float32 bSquared = std::pow(b, 2);
        float32 cSquared = std::pow(c, 2);
        float32 dSquared = std::pow(d, 2);

        return {
            {1 - 2*(cSquared + dSquared), 2*(b*c + a*d), 2*(b*d - a*c), 0},
            {2*(b*c - a*d), 1 - 2*(bSquared + dSquared), 2*(c*d + a*b), 0},
            {2*(b*d + a*c), 2*(c*d - a*b), 1 - 2*(bSquared + cSquared), 0},
            {0, 0, 0, 1}
        };
    }

    void print() const {
        printf("Quat(%f, %f, %f, %f)\n", a, b, c, d);
    }
};

