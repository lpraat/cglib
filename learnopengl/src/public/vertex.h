#pragma once

#include "core_types.h"

namespace glp {

template <typename T = float32>
struct Vertex {
    Vec3<T> Position;
    Vec3<T> Normal;
    Vec2<T> TexCoords;
    Vec3<T> Tangent;
    Vec3<T> Bitangent;
};

}; // namespace glp
