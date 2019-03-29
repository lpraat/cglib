#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "core_types.h"

// TODO use core_types

constexpr double pi() { return std::atan(1)*4; }

template <typename T>
using matrix = std::vector<std::vector<T>>;

matrix<float32> compose(std::vector<matrix<float32>>& matrices);
matrix<float32> dotProduct(const matrix<float32>& m1, const matrix<float32>& m2);

matrix<float32> translationMatrix(float32 dx, float32 dy, float32 dz) {
    return {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1}
    };
}

matrix<float32> scalingMatrix(float32 sx, float32 sy, float32 sz) {
    return {
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> planarMirroringXYMatrix() {
    return scalingMatrix(1, 1, -1);
}

matrix<float32> planarMirroringYZMatrix() {
    return scalingMatrix(-1, 1, 1);
}

matrix<float32> planarMirroringZXMatrix() {
    return scalingMatrix(1, 1, -1);
}

matrix<float32> axialMirroringXMatrix() {
    return scalingMatrix(1, -1, -1);
}

matrix<float32> axialMirroringYMatrix() {
    return scalingMatrix(-1, 1, -1);
}

matrix<float32> axialMirroringZMatrix() {
    return scalingMatrix(-1, -1, 1);
}

matrix<float32> centralMirroringMatrix() {
    return scalingMatrix(-1, -1, -1);
}

matrix<float32> rotationXMatrix(float32 alpha) {
    float32 cosAlpha = std::cos((alpha * pi()) / 180);
    float32 sinAlpha = std::sin((alpha * pi()) / 180);

    return {
        {1, 0, 0, 0},
        {0, cosAlpha, -sinAlpha, 0},
        {0, sinAlpha, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> rotationYMatrix(float32 alpha) {
    float32 cosAlpha = std::cos((alpha * pi()) / 180);
    float32 sinAlpha = std::sin((alpha * pi()) / 180);

    return {
        {cosAlpha, 0, sinAlpha, 0},
        {0, 1, 0, 0},
        {-sinAlpha, 0, cosAlpha, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> rotationZMatrix(float32 alpha) {
    float32 cosAlpha = std::cos((alpha * pi()) / 180);
    float32 sinAlpha = std::sin((alpha * pi()) / 180);

    return {
        {cosAlpha, -sinAlpha, 0, 0},
        {sinAlpha, cosAlpha, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> shearXMatrix(float32 hy, float32 hz) {
    return {
        {1, 0, 0, 0},
        {hy, 1, 0, 0},
        {hz, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> shearYMatrix(float32 hx, float32 hz) {
    return {
        {1, hx, 0, 0},
        {0, 1, 0, 0},
        {0, hz, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> shearZMatrix(float32 hx, float32 hy) {
   return {
        {1, 0, hx, 0},
        {0, 1, hy, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

matrix<float32> orthogonalProjectionMatrix(float32 hw, float32 n, float32 f, float32 a) {
    return {
        {1/hw, 0, 0, 0},
        {0, a/hw, 0, 0},
        {0, 0, -2/(f-n), (f+n)/(n-f)},
        {0, 0, 0, 1}
    };
}

matrix<float32> isometricProjectionMatrix(float32 hw, float32 n, float32 f, float32 a) {
    std::vector<matrix<float32>> toCompose {
        orthogonalProjectionMatrix(hw, n, f, a),
        rotationXMatrix(35.26),
        rotationYMatrix(45)
    };

    return compose(toCompose);
}

matrix<float32> dimetricProjectionMatrix(float32 alpha, float32 hw, float32 n, float32 f, float32 a) {
    std::vector<matrix<float32>> toCompose {
        orthogonalProjectionMatrix(hw, n, f, a),
        rotationXMatrix(alpha),
        rotationYMatrix(45)
    };

    return compose(toCompose);
}

matrix<float32> trimetricProjectionMatrix(float32 alpha, float32 beta, float32 hw, float32 n, float32 f, float32 a) {
    std::vector<matrix<float32>> toCompose {
        orthogonalProjectionMatrix(hw, n, f, a),
        rotationXMatrix(alpha),
        rotationYMatrix(beta)
    };

    return compose(toCompose);
}

matrix<float32> cavalierProjection(float32 alpha, float32 hw, float32 n, float32 f, float32 a) {
    float32 cosAlpha = std::cos((alpha * pi()) / 180);
    float32 sinAlpha = std::sin((alpha * pi()) / 180);

    matrix<float32> orthogonalProjection = orthogonalProjectionMatrix(hw, n, f, a);
    matrix<float32> shearZ = shearZMatrix(-cosAlpha, -sinAlpha);
    return dotProduct(orthogonalProjection, shearZ);
}

matrix<float32> cabinetProjection(float32 alpha, float32 hw, float32 n, float32 f, float32 a) {
    float32 cosAlpha = std::cos((alpha * pi()) / 180);
    float32 sinAlpha = std::sin((alpha * pi()) / 180);

    matrix<float32> orthogonalProjection = orthogonalProjectionMatrix(hw, n, f, a);
    matrix<float32> shearZ = shearZMatrix(-0.5 * cosAlpha, -0.5 * sinAlpha);
    return dotProduct(orthogonalProjection, shearZ);
}

matrix<float32> perspectiveProjection(float32 fov, float32 n, float32 f, float32 a) {
    float32 tanFovHalved = std::tan((fov*pi())/180/2);

    return {
        {1/(a * tanFovHalved), 0, 0, 0},
        {0, 1/tanFovHalved, 0, 0},
        {0, 0, (f+n)/(n-f), 2*f*n/(n-f)},
        {0, 0, -1, 0}
    };
}


// TODO temporary, assume matrix have correct dimensions
matrix<float32> dotProduct(const matrix<float32>& m1, const matrix<float32>& m2) {
    if (m1[0].size() != m2.size()) {
        throw std::runtime_error("Mismatch in matrix dimensions");
    }

    matrix<float32> result(m1.size(), std::vector<float32>(m2[0].size()));
    float32 sum;

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

matrix<float32> compose(std::vector<matrix<float32>>& matrices) {
    matrix<float32> result;

    result = dotProduct(matrices[0], matrices[1]);

    for (int i = 2; i < matrices.size(); i++) {
        result = dotProduct(result, matrices[i]);
    }

    return result;
}