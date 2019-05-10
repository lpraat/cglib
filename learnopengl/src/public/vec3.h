#pragma once

#include "core_types.h"
#include <initializer_list>
#include <iostream>
#include <cmath>

namespace glp {

template <typename T = float32>
struct Vec3 {

    union {
        struct {
            T x, y, z;
        };

        T v[3];
    };

    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    T length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    Vec3<T>& normalize() {
        T l = length();
        x /= l; y /= l; z /= l;
        return *this;
    }

    Vec3<T> normalized() {
        T l = length();
        return {x / l, y / l, z / l};
    }

    Vec3<T> operator-() const {
        return {-x, -y, -z};
    }

    Vec3<T>& operator+=(T s) {
        x += s; y += s; z += s;
        return *this;
    }

    Vec3<T> operator+(T s) const {
        return {x + s, y + s, z + s};
    }

    friend Vec3<T> operator+(T s, const Vec3& v3) {
        return v3 + s;
    }

    Vec3<T>& operator-=(T s) {
        x -= s; y -= s; z -= s;
        return *this;
    }

    Vec3<T> operator-(T s) const {
        return {x - s, y - s, z - s};
    }

    Vec3<T>& operator*=(T s) {
        x *= s; y *= s; z *= s;
        return *this;
    }

    Vec3<T> operator*(T s) const {
        return {x * s, y * s, z * s};
    }

    friend Vec3<T> operator*(T s, const Vec3<T>& v3) {
        return v3 * s;
    }

    Vec3<T>& operator/=(T s) {
        x /= s; y /= s; z /= s;
        return *this;
    }

    Vec3<T> operator/(T s) const {
        return {x / s, y / s, z / s};
    }

    Vec3<T>& operator+=(const Vec3<T>& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3<T> operator+(const Vec3<T>& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3<T>& operator-=(const Vec3<T>& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vec3<T> operator-(const Vec3<T>& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3<T>& operator*=(const Vec3<T>& other) {
        x *= other.x; y *= other.y; z *= other.z;
        return *this;
    }

    Vec3<T> operator*(const Vec3<T>& other) const {
        return {x * other.x, y * other.y, z * other.z};
    }

    Vec3<T>& operator/=(const Vec3<T>& other) {
        x /= other.x; y /= other.y; z /= other.z;
        return *this;
    }

    Vec3<T> operator/(const Vec3<T>& other) const {
        return {x / other.x, y / other.y, z / other.z};
    }

    T dot(const Vec3<T>& other) const {
        return x*other.x + y*other.y + z*other.z;
    }

    Vec3<T> cross(const Vec3<T>& other) const {
        return {y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x};
    }

    void print() const {
        std::cout << "Vec3(" << x << "," << y << "," << z << ")";
    }
};

} // namespace glp