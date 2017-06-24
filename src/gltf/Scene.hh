#pragma once
#include <vector>
#include <string>

namespace gltf {

struct Scene {
protected:
    std::string name;
    std::vector<size_t> nodes;
};

}
