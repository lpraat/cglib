#pragma once

#include <vector>
#include <cmath>
#include <iostream>

// TODO use core_types

constexpr double pi() { return std::atan(1)*4; }

template <typename T>
using matrix = std::vector<std::vector<T>>;

matrix<float> translationMatrix(float dx, float dy, float dz) {
    return {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1}
    };
}

matrix<float> scalingMatrix(float sx, float sy, float sz) {
    return {
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> planarMirroringXYMatrix() {
    return scalingMatrix(1, 1, -1);
}

matrix<float> planarMirroringYZMatrix() {
    return scalingMatrix(-1, 1, 1);
}

matrix<float> planarMirroringZXMatrix() {
    return scalingMatrix(1, 1, -1);
}

matrix<float> axialMirroringXMatrix() {
    return scalingMatrix(1, -1, -1);
}

matrix<float> axialMirroringYMatrix() {
    return scalingMatrix(-1, 1, -1);
}

matrix<float> axialMirroringZMatrix() {
    return scalingMatrix(-1, -1, 1);
}

matrix<float> centralMirroringMatrix() {
    return scalingMatrix(-1, -1, -1);
}

matrix<float> rotationXMatrix(float alpha) {
    float cosAlpha = std::cos((alpha * pi()) / 180);
    float sinAlpha = std::sin((alpha * pi() / 180));

    return {
        {1, 0, 0, 0},
        {0, cosAlpha, -sinAlpha, 0},
        {0, sinAlpha, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> rotationYMatrix(float alpha) {
    float cosAlpha = std::cos((alpha * pi()) / 180);
    float sinAlpha = std::sin((alpha * pi() / 180));

    return {
        {cosAlpha, 0, sinAlpha, 0},
        {0, 1, 0, 0},
        {-sinAlpha, 0, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> rotationZMatrix(float alpha) {
    float cosAlpha = std::cos((alpha * pi()) / 180);
    float sinAlpha = std::sin((alpha * pi() / 180));

    return {
        {cosAlpha, -sinAlpha, 0, 0},
        {sinAlpha, cosAlpha, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> shearXMatrix(float hy, float hz) {
    return {
        {1, 0, 0, 0},
        {hy, 1, 0, 0},
        {hz, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> shearYMatrix(float hx, float hz) {
    return {
        {1, hx, 0, 0},
        {0, 1, 0, 0},
        {0, hz, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float> shearZMatrix(float hx, float hy) {
   return {
        {1, 0, hx, 0},
        {0, 1, hy, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}


// TODO temporary, assume matrix have correct dimensions
matrix<float> dotProduct(const matrix<float>& m1, const matrix<float>& m2) {

    if (m1[0].size() != m2.size()) {
        throw std::runtime_error("Mismatch in matrix dimensions");
    }

    matrix<float> result(m1.size(), std::vector<float>(m2[0].size()));
    float sum;

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m2[0].size(); j++) {

            sum = 0;
            for (int k = 0; k < m2.size(); k++) {
                sum += m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

matrix<float> compose(std::vector<matrix<float>>& matrices) {
    matrix<float> result;

    result = dotProduct(matrices[0], matrices[1]);

    for (int i = 2; i < matrices.size(); i++) {
        result = dotProduct(result, matrices[i]);
    }

    return result;
}