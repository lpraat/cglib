#pragma once

#include "core_types.h"
#include <iostream>
#include <cmath>

namespace cglib {

template <typename T = float32>
struct Vec2 {

    union {
        struct {
            T x, y;
        };

        T v[2];
    };

    Vec2() {}
    Vec2(T x, T y) : x(x), y(y) {}

    T* getPtr() {
        return &v[0];
    }

    T length() const {
        return std::sqrt(x*x + y*y);
    }

    Vec2<T>& normalize() {
        const T l = length();
        x /= l; y /= l;
        return *this;
    }

    Vec2<T> normalized() {
        const T l = length();
        return {x / l, y / l};
    }

    Vec2<T> operator-() const {
        return {-x, -y};
    }

    Vec2<T>& operator+=(T s) {
        x += s; y += s;
        return *this;
    }

    Vec2<T> operator+(T s) const {
        return {x + s, y + s};
    }

    friend Vec2<T> operator+(T s, const Vec2& v2) {
        return v2 + s;
    }

    Vec2<T>& operator-=(T s) {
        x -= s; y -= s;
        return *this;
    }

    Vec2<T> operator-(T s) const {
        return {x - s, y - s};
    }

    Vec2<T>& operator*=(T s) {
        x *= s; y *= s;
        return *this;
    }

    Vec2<T> operator*(T s) const {
        return {x * s, y * s};
    }

    friend Vec2<T> operator*(T s, const Vec2<T>& v2) {
        return v2 * s;
    }

    Vec2<T>& operator/=(T s) {
        x /= s; y /= s;
        return *this;
    }

    Vec2<T> operator/(T s) const {
        return {x / s, y / s};
    }

    Vec2<T>& operator+=(const Vec2<T>& other) {
        x += other.x; y += other.y;
        return *this;
    }

    Vec2<T> operator+(const Vec2<T>& other) const {
        return {x + other.x, y + other.y};
    }

    Vec2<T>& operator-=(const Vec2<T>& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    Vec2<T> operator-(const Vec2<T>& other) const {
        return {x - other.x, y - other.y};
    }

    Vec2<T>& operator*=(const Vec2<T>& other) {
        x *= other.x; y *= other.y;
        return *this;
    }

    Vec2<T> operator*(const Vec2<T>& other) const {
        return {x * other.x, y * other.y};
    }

    Vec2<T>& operator/=(const Vec2<T>& other) {
        x /= other.x; y /= other.y;
        return *this;
    }

    Vec2<T> operator/(const Vec2<T>& other) const {
        return {x / other.x, y / other.y};
    }

    T dot(const Vec2<T>& other) const {
        return x*other.x + y*other.y;
    }

    void print() const {
        std::cout << "Vec2(" << x << "," << y << ")";
    }
};

} // namespace cglib