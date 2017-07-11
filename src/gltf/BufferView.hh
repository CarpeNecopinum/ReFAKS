#pragma once
#include <cstdlib>

namespace gltf {

struct BufferView {
    size_t buffer;
    size_t byteLength;
    size_t byteOffset;
    size_t byteStride = 0;
    uint   target;
};

}
