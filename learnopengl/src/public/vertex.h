#pragma once

#include "core_types.h"
#include "vec3.h"
#include "vec2.h"

namespace glp {

template <typename T = float32>
struct Vertex {
    Vec3<T> Position;
    Vec3<T> Normal;
    Vec2<T> TexCoords;
    Vec3<T> Tangent;
};

}; // namespace glp
