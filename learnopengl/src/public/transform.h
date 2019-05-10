#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "core_types.h"
#include "constants.h"
#include "mat4.h"
#include "vec3.h"

namespace glp {

// Forward
template <typename T, typename S = uint8>
Mat4<T> compose(const std::vector<Mat4<T>>& matrices);


template <typename T = float32>
Mat4<T> translate(T dx, T dy, T dz) {
    return {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> translate(const Vec3<T>& d) {
    return {
        {1, 0, 0, d.x},
        {0, 1, 0, d.y},
        {0, 0, 1, d.z},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> scale(T sx, T sy, T sz) {
    return {
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> scale(T s) {
    return {
        {s, 0, 0, 0},
        {0, s, 0, 0},
        {0, 0, s, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> scale(const Vec3<T>& s) {
    return {
        {s.x, 0, 0, 0},
        {0, s.y, 0, 0},
        {0, 0, s.z, 0},
        {0, 0, 0, 1}
    };
}


template <typename T = float32>
Mat4<T> planarMirrorXY() {
    return scale<T>(1, 1, -1);
}

template <typename T = float32>
Mat4<T> planarMirrorYZ() {
    return scale<T>(-1, 1, 1);
}

template <typename T = float32>
Mat4<T> planarMirrorZX() {
    return scale<T>(1, 1, -1);
}

template <typename T = float32>
Mat4<T> axialMirrorX() {
    return scale<T>(1, -1, -1);
}

template <typename T = float32>
Mat4<T> axialMirrorY() {
    return scale<T>(-1, 1, -1);
}

template <typename T = float32>
Mat4<T> axialMirrorZ() {
    return scale<T>(-1, -1, 1);
}

template <typename T = float32>
Mat4<T> centralMirror() {
    return scale<T>(-1, -1, -1);
}

template <typename T = float32>
Mat4<T> rotateX(T alpha) {
    const T cosAlpha = std::cos(alpha * toRadians<T>());
    const T sinAlpha = std::sin(alpha * toRadians<T>());

    return {
        {1, 0, 0, 0},
        {0, cosAlpha, -sinAlpha, 0},
        {0, sinAlpha, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> rotateY(T alpha) {
    const T cosAlpha = std::cos(alpha * toRadians<T>());
    const T sinAlpha = std::sin(alpha * toRadians<T>());

    return {
        {cosAlpha, 0, sinAlpha, 0},
        {0, 1, 0, 0},
        {-sinAlpha, 0, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> rotateZ(T alpha) {
    const T cosAlpha = std::cos(alpha * toRadians<T>());
    const T sinAlpha = std::sin(alpha * toRadians<T>());

    return {
        {cosAlpha, -sinAlpha, 0, 0},
        {sinAlpha, cosAlpha, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> shearX(T hy, T hz) {
    return {
        {1, 0, 0, 0},
        {hy, 1, 0, 0},
        {hz, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> shearY(T hx, T hz) {
    return {
        {1, hx, 0, 0},
        {0, 1, 0, 0},
        {0, hz, 1, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> shearZ(T hx, T hy) {
   return {
        {1, 0, hx, 0},
        {0, 1, hy, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> orthogonalProjection(T l, T r, T b, T t, T n, T f) {
    return {
        {2/(r-l), 0, 0, -(l+r)/(r-l)},
        {0, 2/(t-b), 0, -(t+b)/(t-b)},
        {0, 0, -2/(f-n), -(f+n)/(f-n)},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> orthogonalProjection(T hw, T n, T f, T a) {
    return {
        {1/hw, 0, 0, 0},
        {0, a/hw, 0, 0},
        {0, 0, -2/(f-n), (f+n)/(n-f)},
        {0, 0, 0, 1}
    };
}

template <typename T = float32>
Mat4<T> isometricProjection(T hw, T n, T f, T a) {
    const std::vector<Mat4<T>> toCompose {
        orthogonalProjection<T>(hw, n, f, a),
        rotateX<T>(35.26),
        rotateY<T>(45)
    };
    return compose<T>(toCompose);
}

template <typename T = float32>
Mat4<T> dimetricProjection(T alpha, T hw, T n, T f, T a) {
    const std::vector<Mat4<T>> toCompose {
        orthogonalProjection<T>(hw, n, f, a),
        rotateX<T>(alpha),
        rotateY<T>(45)
    };
    return compose<T>(toCompose);
}

template <typename T = float32>
Mat4<T> trimetricProjection(T alpha, T beta, T hw, T n, T f, T a) {
    const std::vector<Mat4<T>> toCompose {
        orthogonalProjection<T>(hw, n, f, a),
        rotateX<T>(alpha),
        rotateY<T>(beta)
    };
    return compose<T>(toCompose);
}

template <typename T = float32>
Mat4<T> cavalierProjection(T alpha, T hw, T n, T f, T a) {
    const T cosAlpha = std::cos(alpha * toRadians<T>());
    const T sinAlpha = std::sin(alpha * toRadians<T>());
    return orthogonalProjection<T>(hw, n, f, a).dot(shearZ<T>(-cosAlpha, -sinAlpha));
}

template <typename T = float32>
Mat4<T> cabinetProjection(T alpha, T hw, T n, T f, T a) {
    const T cosAlpha = std::cos(alpha * toRadians<T>());
    const T sinAlpha = std::sin(alpha * toRadians<T>());
    return orthogonalProjection<T>(hw, n, f, a).dot(shearZ<T>(-0.5 * cosAlpha, -0.5 * sinAlpha));
}

template <typename T = float32>
Mat4<T> perspectiveProjection(T fov, T n, T f, T a) {
    const T tanFovHalved = std::tan((fov * toRadians<T>()) / 2);

    return {
        {1/(a * tanFovHalved), 0, 0, 0},
        {0, 1/tanFovHalved, 0, 0},
        {0, 0, (f+n)/(n-f), 2*f*n/(n-f)},
        {0, 0, -1, 0}
    };
}

template <typename T = float32>
Mat4<T> lookAt(const Vec3<T>& position, const Vec3<T>& target, const Vec3<T>& up) {
    const Vec3<T> vz = (position - target).normalize();
    const Vec3<T> vx = up.cross(vz).normalize();
    const Vec3<T> vy = vz.cross(vx);

    return {
        {vx.x, vx.y, vx.z, -vx.dot(position)},
        {vy.x, vy.y, vy.z, -vy.dot(position)},
        {vz.x, vz.y, vz.z, -vz.dot(position)},
        {0, 0, 0, 1}
    };
}

template <typename T = float32, typename S = uint8>
Mat4<T> compose(std::vector<Mat4<T>>& matrices) {
    Mat4<T> result {matrices[0].dot(matrices[1])};

    for (S i = 2; i < matrices.size(); i++) {
        result = result.dot(matrices[i]);
    }

    return result;
}

/**
 * Extracting Euler Angles from a Rotation Matrix by Mike Day, Insomniac Games
 * https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles1.pdf
*/
template <typename T = float32>
Vec3<T> extractEulerAngles(Mat4<T>& rotMatrix) {
    T theta1 = std::atan2(rotMatrix(2, 1), rotMatrix(2, 2));
    T s1 = std::sin(theta1);
    T c1 = std::cos(theta1);
    T c2 = std::sqrt(rotMatrix(0, 0)*rotMatrix(0, 0) + rotMatrix(1, 0)*rotMatrix(1, 0));
    T theta2 = std::atan2(-rotMatrix(2, 0), c2);
    T theta3 = std::atan2(s1*rotMatrix(0, 2) - c1*rotMatrix(0, 1), c1*rotMatrix(1, 1) - s1*rotMatrix(1, 2));

    return {theta1, theta2, theta3};
}

}; // namespace glp