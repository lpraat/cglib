#pragma once

#include "core_types.h"
#include "mesh.h"

namespace cglib {

template <typename T>
class Mesh;

template <typename T = float32>
struct Node {
    std::string name;
    std::vector<Mesh<T>> meshes;
    std::vector<Node<T>*> children;
    Mat4<T> model = Mat4<T>::identity();
};

}; // namespace cglib