#pragma once

#include "core_types.h"
#include <initializer_list>
#include <iostream>
#include <cmath>
#include "vec3.h"

namespace glp {

template <typename T = float32>
struct Vec4 {

    union {
        struct {
            T x, y, z, w;
        };

        T v[4];
    };

    Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    T length() const {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }

    Vec4<T>& normalize() {
        T l = length();
        x /= l; y /= l; z /= l; w /= w;
        return *this;
    }

    Vec4<T> normalized() {
        T l = length();
        return {x / l, y / l, z / l, w / l};
    }

    Vec4<T> operator-() const {
        return {-x, -y, -z, -w};
    }

    Vec4<T>& operator+=(T s) {
        x += s; y += s; z += s; w += s;
        return *this;
    }

    Vec4<T> operator+(T s) const {
        return {x + s, y + s, z + s, w + s};
    }

    friend Vec4<T> operator+(T s, const Vec4& v4) {
        return v4 + s;
    }

    Vec4<T>& operator-=(T s) {
        x -= s; y -= s; z -= s; w -= s;
        return *this;
    }

    Vec4<T> operator-(T s) const {
        return {x - s, y - s, z - s, w - s};
    }

    Vec4<T>& operator*=(T s) {
        x *= s; y *= s; z *= s; w *= s;
        return *this;
    }

    Vec4<T> operator*(T s) const {
        return {x * s, y * s, z * s, w * s};
    }

    friend Vec4<T> operator*(T s, const Vec4& v4) {
        return v4 * s;
    }

    Vec4<T>& operator/=(T s) {
        x /= s; y /= s; z /= s; w /= s;
        return *this;
    }

    Vec4<T> operator/(T s) const {
        return {x / s, y / s, z / s, w / s};
    }

    Vec4<T>& operator+=(const Vec4<T>& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    Vec4<T> operator+(const Vec4<T>& other) const {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Vec4<T>& operator-=(const Vec4<T>& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    Vec4<T> operator-(const Vec4<T>& other) const {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Vec4<T>& operator*=(const Vec4<T>& other) {
        x *= other.x; y *= other.y; z *= other.z; w *= other.w;
        return *this;
    }

    Vec4<T> operator*(const Vec4<T>& other) const {
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }

    Vec4<T>& operator/=(const Vec4<T>& other) {
        x /= other.x; y /= other.y; z /= other.z; w /= other.w;
        return *this;
    }

    Vec4<T> operator/(const Vec4<T>& other) const {
        return {x / other.x, y / other.y, z / other.z, w / other.w};
    }

    T dot(const Vec4<T>& other) const {
        return x*other.x + y*other.y + z*other.z + w*other.w;
    }

    void print() const {
        std::cout << "Vec4(" << x << "," << y << "," << z << "," << w << ")";
    }
};

} // namespace glp