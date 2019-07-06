#pragma once

#include "core_types.h"
#include <initializer_list>
#include <iostream>
#include "vec4.h"

namespace glp {

template <typename T = float32>
struct Mat3 {

    union {
        struct {
            T x0, y0, z0;
            T x1, y1, z1;
            T x2, y2, z2;
        };

        T v[9];
    };

    Mat3(const std::initializer_list<std::initializer_list<T>>& l) {
        uint8 i = 0;
        for (auto& row : l) {
            for (auto& el : row) {
                v[i] = el;
                i++;
            }
        }
    }

    static Mat3<T> identity() {
        return {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
    }

    T& operator()(uint8 i, uint8 j) {
        return v[i*3 + j];
    }

    T* getPtr() {
        return &v[0];
    }

    Mat3<T> transposedInverse() {
        T determinant = x0*(y1*z2 - y2*z1) - y0*(x1*z2 - z1*x2) + z0*(x1*y2 - y1*x2);
        T invdet = 1/determinant;

        return {
            {(y1*z2 - y2*z1)*invdet, -(x1*z2 - z1*x2)*invdet, (x1*y2 - x2*y1)*invdet},
            {-(y0*z2 - z0*y2)*invdet, (x0*z2 - z0*x2)*invdet, -(x0*y2 - x2*y0)*invdet},
            {(y0*z1 - z0*y1)*invdet, -(x0*z1 - x1*z0)*invdet,  (x0*y1 - x1*y0)*invdet}
        };
    }


    void print() const {
        uint8 i;
        std::cout << "Mat3(";
        for (i = 0; i < 8; i++) {
            std::cout << v[i] << ", ";
        }
        std::cout << v[i] << ")";
    }

};
}; // namespace glp