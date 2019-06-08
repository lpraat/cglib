#pragma once

#include "vec3.h"

namespace glp {

template <typename T = float32>
struct PointLight {
    Vec3<T> position;

    Vec3<T> ambient;
    Vec3<T> diffuse;
    Vec3<T> specular;

    T constant;
    T linear;
    T quadratic;

    T cutOff;
    T outerCutOff;
};

}; // namespace glp