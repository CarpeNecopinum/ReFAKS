#pragma once

#include "Forward.hh"
#include <vector>

namespace gltf {

class Scene {
protected:
    std::vector<SharedNode> mNodes;
};

}
