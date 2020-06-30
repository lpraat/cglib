#pragma once

#include "vec3.h"

namespace cglib {

template <typename T = float32>
struct DirectionalLight {
    Vec3<T> ambient;
    Vec3<T> diffuse;
    Vec3<T> specular;
    Vec3<T> dir; // specified as FROM the source
};

}; // namespace cglib