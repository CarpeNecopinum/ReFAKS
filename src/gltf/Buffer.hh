#pragma once
#include <string>
#include <experimental/optional>

namespace gltf {

struct Buffer {
    size_t byteLength;
    std::experimental::optional<std::string> uri;
};

}
