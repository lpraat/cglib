#pragma once

#include "core_types.h"
#include <initializer_list>
#include <iostream>
#include "vec4.h"
#include "mat3.h"

namespace glp {

template <typename T = float32>
struct Mat4 {

    union {
        struct {
            T x0, y0, z0, w0;
            T x1, y1, z1, w1;
            T x2, y2, z2, w2;
            T x3, y3, z3, w3;
        };

        T v[16];
    };

    Mat4(const std::initializer_list<std::initializer_list<T>>& l) {
        uint8 i = 0;
        for (auto& row : l) {
            for (auto& el : row) {
                v[i] = el;
                i++;
            }
        }
    }

    static Mat4<T> identity() {
        return {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
    }

    T& operator()(uint8 i, uint8 j) {
        return v[(i << 2) + j];
    }

    T* getPtr() {
        return &v[0];
    }

    Mat4<T>& operator+=(T s) {
        x0 += s; y0 += s; z0 += s; w0 += s;
        x1 += s; y1 += s; z1 += s; w1 += s;
        x2 += s; y2 += s; z2 += s; w2 += s;
        x3 += s; y3 += s; z3 += s; w3 += s;
        return *this;
    }

    Mat4<T> operator+(T s) const {
        return {
            {x0 + s, y0 + s, z0 + s, w0 + s},
            {x1 + s, y1 + s, z1 + s, w1 + s},
            {x2 + s, y2 + s, z2 + s, w2 + s},
            {x3 + s, y3 + s, z3 + s, w3 + s}
        };
    }

    friend Mat4<T> operator+(T s, const Mat4& m4) {
        return {
            {m4.x0 + s, m4.y0 + s, m4.z0 + s, m4.w0 + s},
            {m4.x1 + s, m4.y1 + s, m4.z1 + s, m4.w1 + s},
            {m4.x2 + s, m4.y2 + s, m4.z2 + s, m4.w2 + s},
            {m4.x3 + s, m4.y3 + s, m4.z3 + s, m4.w3 + s}
        };
    }

    Mat4<T>& operator-=(T s) {
        x0 -= s; y0 += s; z0 += s; w0 += s;
        x1 -= s; y1 += s; z1 += s; w1 += s;
        x2 -= s; y2 += s; z2 += s; w2 += s;
        x3 -= s; y3 += s; z3 += s; w3 += s;
        return *this;
    }

    Mat4<T> operator-(T s) const {
        return {
            {x0 - s, y0 - s, z0 - s, w0 - s},
            {x1 - s, y1 - s, z1 - s, w1 - s},
            {x2 - s, y2 - s, z2 - s, w2 - s},
            {x3 - s, y3 - s, z3 - s, w3 - s}
        };
    }

    Mat4<T>& operator*=(T s) {
        x0 *= s; y0 *= s; z0 *= s; w0 *= s;
        x1 *= s; y1 *= s; z1 *= s; w1 *= s;
        x2 *= s; y2 *= s; z2 *= s; w2 *= s;
        x3 *= s; y3 *= s; z3 *= s; w3 *= s;
        return *this;
    }

    Mat4<T> operator*(T s) const {
        return {
            {x0 * s, y0 * s, z0 * s, w0 * s},
            {x1 * s, y1 * s, z1 * s, w1 * s},
            {x2 * s, y2 * s, z2 * s, w2 * s},
            {x3 * s, y3 * s, z3 * s, w3 * s}
        };
    }

    friend Mat4<T> operator*(T s, const Mat4& m4) {
        return {
            {m4.x0 * s, m4.y0 * s, m4.z0 * s, m4.w0 * s},
            {m4.x1 * s, m4.y1 * s, m4.z1 * s, m4.w1 * s},
            {m4.x2 * s, m4.y2 * s, m4.z2 * s, m4.w2 * s},
            {m4.x3 * s, m4.y3 * s, m4.z3 * s, m4.w3 * s}
        };
    }

    Mat4<T>& operator/=(T s) {
        x0 /= s; y0 /= s; z0 /= s; w0 /= s;
        x1 /= s; y1 /= s; z1 /= s; w1 /= s;
        x2 /= s; y2 /= s; z2 /= s; w2 /= s;
        x3 /= s; y3 /= s; z3 /= s; w3 /= s;
        return *this;
    }

    Mat4<T> operator/(T s) const {
        return {
            {x0 / s, y0 / s, z0 / s, w0 / s},
            {x1 / s, y1 / s, z1 / s, w1 / s},
            {x2 / s, y2 / s, z2 / s, w2 / s},
            {x3 / s, y3 / s, z3 / s, w3 / s}
        };
    }

    Mat4<T>& operator+=(const Mat4<T>& other) {
        x0 += other.x0; y0 += other.y0; z0 += other.z0; w0 += other.w0;
        x1 += other.x1; y1 += other.y1; z1 += other.z1; w1 += other.w1;
        x2 += other.x2; y2 += other.y2; z2 += other.z2; w2 += other.w2;
        x3 += other.x3; y3 += other.y3; z3 += other.z3; w3 += other.w3;
        return *this;
    }

    Mat4<T>& operator+(const Mat4<T>& other) const {
        return {
            {x0 + other.x0, y0 + other.y0, z0 + other.z0, w0 + other.w0},
            {x1 + other.x1, y1 + other.y1, z1 + other.z1, w1 + other.w1},
            {x2 + other.x2, y2 + other.y2, z2 + other.z2, w2 + other.w2},
            {x3 + other.x3, y3 + other.y3, z3 + other.z3, w0 + other.w3},
        };
    }

    Mat4<T>& operator-=(const Mat4<T>& other) {
        x0 -= other.x0; y0 -= other.y0; z0 -= other.z0; w0 -= other.w0;
        x1 -= other.x1; y1 -= other.y1; z1 -= other.z1; w1 -= other.w1;
        x2 -= other.x2; y2 -= other.y2; z2 -= other.z2; w2 -= other.w2;
        x3 -= other.x3; y3 -= other.y3; z3 -= other.z3; w3 -= other.w3;
        return *this;
    }

    Mat4<T>& operator-(const Mat4<T>& other) const {
        return {
            {x0 - other.x0, y0 - other.y0, z0 - other.z0, w0 - other.w0},
            {x1 - other.x1, y1 - other.y1, z1 - other.z1, w1 - other.w1},
            {x2 - other.x2, y2 - other.y2, z2 - other.z2, w2 - other.w2},
            {x3 - other.x3, y3 - other.y3, z3 - other.z3, w0 - other.w3},
        };
    }

    Mat4<T>& operator*=(const Mat4<T>& other) {
        x0 *= other.x0; y0 *= other.y0; z0 *= other.z0; w0 *= other.w0;
        x1 *= other.x1; y1 *= other.y1; z1 *= other.z1; w1 *= other.w1;
        x2 *= other.x2; y2 *= other.y2; z2 *= other.z2; w2 *= other.w2;
        x3 *= other.x3; y3 *= other.y3; z3 *= other.z3; w3 *= other.w3;
        return *this;
    }

    Mat4<T>& operator*(const Mat4<T>& other) const {
        return {
            {x0 * other.x0, y0 * other.y0, z0 * other.z0, w0 * other.w0},
            {x1 * other.x1, y1 * other.y1, z1 * other.z1, w1 * other.w1},
            {x2 * other.x2, y2 * other.y2, z2 * other.z2, w2 * other.w2},
            {x3 * other.x3, y3 * other.y3, z3 * other.z3, w0 * other.w3},
        };
    }

    Mat4<T>& operator/=(const Mat4<T>& other) {
        x0 /= other.x0; y0 /= other.y0; z0 /= other.z0; w0 /= other.w0;
        x1 /= other.x1; y1 /= other.y1; z1 /= other.z1; w1 /= other.w1;
        x2 /= other.x2; y2 /= other.y2; z2 /= other.z2; w2 /= other.w2;
        x3 /= other.x3; y3 /= other.y3; z3 /= other.z3; w3 /= other.w3;
        return *this;
    }


    Mat4<T>& operator/(const Mat4<T>& other) const {
        return {
            {x0 / other.x0, y0 / other.y0, z0 / other.z0, w0 / other.w0},
            {x1 / other.x1, y1 / other.y1, z1 / other.z1, w1 / other.w1},
            {x2 / other.x2, y2 / other.y2, z2 / other.z2, w2 / other.w2},
            {x3 / other.x3, y3 / other.y3, z3 / other.z3, w0 / other.w3},
        };
    }

    Mat4<T> dot(const Mat4<T>& other) const {
        return {
            {
                x0*other.x0 + y0*other.x1 + z0*other.x2 + w0*other.x3,
                x0*other.y0 + y0*other.y1 + z0*other.y2 + w0*other.y3,
                x0*other.z0 + y0*other.z1 + z0*other.z2 + w0*other.z3,
                x0*other.w0 + y0*other.w1 + z0*other.w2 + w0*other.w3
            },
            {
                x1*other.x0 + y1*other.x1 + z1*other.x2 + w1*other.x3,
                x1*other.y0 + y1*other.y1 + z1*other.y2 + w1*other.y3,
                x1*other.z0 + y1*other.z1 + z1*other.z2 + w1*other.z3,
                x1*other.w0 + y1*other.w1 + z1*other.w2 + w1*other.w3
            },
            {
                x2*other.x0 + y2*other.x1 + z2*other.x2 + w2*other.x3,
                x2*other.y0 + y2*other.y1 + z2*other.y2 + w2*other.y3,
                x2*other.z0 + y2*other.z1 + z2*other.z2 + w2*other.z3,
                x2*other.w0 + y2*other.w1 + z2*other.w2 + w2*other.w3
            },
            {
                x3*other.x0 + y3*other.x1 + z3*other.x2 + w3*other.x3,
                x3*other.y0 + y3*other.y1 + z3*other.y2 + w3*other.y3,
                x3*other.z0 + y3*other.z1 + z3*other.z2 + w3*other.z3,
                x3*other.w0 + y3*other.w1 + z3*other.w2 + w3*other.w3
            }
        };
    }

    Mat3<T> mat3() const {
        return {
            {x0, y0, z0},
            {x1, y1, z1},
            {x2, y2, z2}
        };
    }

    Vec4<T> dot(const Vec4<T>& other) const {
        return {
            x0*other.x + y0*other.y + z0*other.z + w0*other.w,
            x1*other.x + y1*other.y + z1*other.z + w1*other.w,
            x2*other.x + y2*other.y + z2*other.z + w2*other.w,
            x3*other.x + y3*other.y + z3*other.z + w3*other.w,
        };
    }

    void print() const {
        uint8 i;
        std::cout << "Mat4(";
        for (i = 0; i < 15; i++) {
            std::cout << v[i] << ", ";
        }
        std::cout << v[i] << ")";
    }

};
}; // namespace glp