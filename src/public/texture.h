#pragma once

#include "core_types.h"

#include <string>

namespace cglib {
struct Texture {
    uint32 id;
    std::string type;
    std::string path;
};

}; // namespace cglib