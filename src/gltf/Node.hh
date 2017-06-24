#pragma once
#include <vector>
#include <glm/mat4x4.hpp>

namespace gltf {

struct Node {
    std::vector<size_t> children;
    glm::mat4x4 matrix;
};

}
