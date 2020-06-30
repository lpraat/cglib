#pragma once

#include <cmath>
#include "core_types.h"

namespace cglib {

template <typename T = float32>
constexpr T pi() { return std::atan(1)*4; }

template <typename T = float32>
constexpr T toRadians() { return pi<T>() / 180; }

template <typename T = float32>
constexpr T toDegrees() { return 180 / pi<T>(); }

}; // namespace cglib