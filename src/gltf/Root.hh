#pragma once
#include "Forward.hh"
#include <vector>

namespace gltf {

struct Root {
    std::vector<UniqueScene>  scenes;
    std::vector<UniqueNode>   nodes;
    std::vector<UniqueCamera> cameras;
    size_t scene;
};

}
