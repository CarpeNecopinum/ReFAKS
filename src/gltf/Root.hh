#pragma once
#include "Node.hh"
#include "Scene.hh"
#include "Camera.hh"
#include "Buffer.hh"
#include "BufferView.hh"
#include "Accessor.hh"
#include <vector>

namespace gltf {

struct Root {
    std::vector<Node>   nodes;
    std::vector<Scene>  scenes;
    std::vector<Camera> cameras;
    std::vector<Buffer> buffers;
    std::vector<BufferView> bufferViews;
    std::vector<Accessor> accessors;
    size_t scene;
};

}
