#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "core_types.h"
#include "constants.h"

template <typename T>
using matrix = std::vector<std::vector<T>>;


// Forward
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

matrix<float32> dotProduct(const matrix<float32>& m1, const matrix<float32>& m2) {
    return {
        {
            m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0] + m1[0][3]*m2[3][0],
            m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1] + m1[0][3]*m2[3][1],
            m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2] + m1[0][3]*m2[3][2],
            m1[0][0]*m2[0][3] + m1[0][1]*m2[1][3] + m1[0][2]*m2[2][3] + m1[0][3]*m2[3][3]
        },
        {
            m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0] + m1[1][3]*m2[3][0],
            m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1] + m1[1][3]*m2[3][1],
            m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2] + m1[1][3]*m2[3][2],
            m1[1][0]*m2[0][3] + m1[1][1]*m2[1][3] + m1[1][2]*m2[2][3] + m1[1][3]*m2[3][3]
        },
        {
            m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0] + m1[2][3]*m2[3][0],
            m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1] + m1[2][3]*m2[3][1],
            m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2] + m1[2][3]*m2[3][2],
            m1[2][0]*m2[0][3] + m1[2][1]*m2[1][3] + m1[2][2]*m2[2][3] + m1[2][3]*m2[3][3]
        },
        {
            m1[3][0]*m2[0][0] + m1[3][1]*m2[1][0] + m1[3][2]*m2[2][0] + m1[3][3]*m2[3][0],
            m1[3][0]*m2[0][1] + m1[3][1]*m2[1][1] + m1[3][2]*m2[2][1] + m1[3][3]*m2[3][1],
            m1[3][0]*m2[0][2] + m1[3][1]*m2[1][2] + m1[3][2]*m2[2][2] + m1[3][3]*m2[3][2],
            m1[3][0]*m2[0][3] + m1[3][1]*m2[1][3] + m1[3][2]*m2[2][3] + m1[3][3]*m2[3][3]
        }
    };
}

std::vector<float32> dotProduct(const matrix<float32>& m1, const std::vector<float32>& v) {
    return {
            m1[0][0]*v[0] + m1[0][1]*v[1] + m1[0][2]*v[2] + m1[0][3]*v[3],
            m1[1][0]*v[0] + m1[1][1]*v[1] + m1[1][2]*v[2] + m1[1][3]*v[3],
            m1[2][0]*v[0] + m1[2][1]*v[1] + m1[2][2]*v[2] + m1[2][3]*v[3],
            m1[3][0]*v[0] + m1[3][1]*v[1] + m1[3][2]*v[2] + m1[3][3]*v[3]
    };
}

matrix<float32> compose(std::vector<matrix<float32>>& matrices) {
    matrix<float32> result;

    result = dotProduct(matrices[0], matrices[1]);

    for (uint32 i = 2; i < matrices.size(); i++) {
        result = dotProduct(result, matrices[i]);
    }

    return result;
}

// Computing Euler angles from a rotation matrix - Gregory G. Slabaugh
std::vector<std::pair<float32, float32>> extractEulerAngles(matrix<float32> rotMatrix) {
    float32 theta1, theta2;
    float32 psi1, psi2;
    float32 fi1, fi2;

    if (rotMatrix[2][0] != -1 && rotMatrix[2][0] != 1) {
        theta1 = -std::asin(rotMatrix[2][0]);
        theta2 = pi() - theta1;

        float32 cosTheta1 = std::cos(theta1);
        float32 cosTheta2 = std::cos(theta2);

        psi1 = std::atan2(rotMatrix[2][1] / cosTheta1, rotMatrix[2][2] / cosTheta1);
        psi2 = std::atan2(rotMatrix[2][1] / cosTheta2, rotMatrix[2][2] / cosTheta2);

        fi1 = std::atan2(rotMatrix[1][0] / cosTheta1, rotMatrix[0][0] / cosTheta1);
        fi2 = std::atan2(rotMatrix[1][0] / cosTheta2, rotMatrix[0][0] / cosTheta2);
    } else {
        fi1 = fi2 = 0;

        if (rotMatrix[2][0] != -1) {
            theta1 = theta2 = pi() / 2;
            psi1 = psi2 = atan2(rotMatrix[0][1], rotMatrix[0][2]);
        } else {
            theta1 = theta2 = - pi() / 2;
            psi1 = psi2 = atan2(- rotMatrix[0][1], - rotMatrix[0][2]);
        }
    }

    return std::vector {std::make_pair(theta1, theta2), std::make_pair(psi1, psi2), std::make_pair(fi1, fi2)};
}

// Extracting Euler Angles from a Rotation Matrix, Mike Day, Insomniac Games
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles1.pdf
std::vector<float32> extractEulerAnglesAlt(matrix<float32> rotMatrix) {
    float32 theta1 = std::atan2(rotMatrix[1][2], rotMatrix[2][2]);
    float32 s1 = std::sin(theta1);
    float32 c1 = std::cos(theta1);
    float32 c2 = std::sqrt(std::pow(rotMatrix[0][0], 2) + std::pow(rotMatrix[0][1], 2));
    float32 theta2 = std::atan2(-rotMatrix[0][2], c2);
    float32 theta3 = std::atan2(s1*rotMatrix[2][0] - c1*rotMatrix[1][0], c1*rotMatrix[1][1] - s1*rotMatrix[2][1]);

    // TODO change euler struct
    return std::vector {
        static_cast<float32>(theta1),
        static_cast<float32>(theta2),
        static_cast<float32>(theta3)
    };
}