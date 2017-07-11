#pragma once
#include <experimental/optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace gltf {

class MeshPrimitive {
    enum class DrawMode : uint {
        POINTS = 0,
        LINES = 1,
        LINE_LOOP = 2,
        LINE_STRIP = 3,
        TRIANGLES = 4,
        TRIANGLE_STRIP = 5,
        TRIANGLE_FAN = 6
    };

    using Attributes = std::unordered_map<std::string, size_t>;

    Attributes attributes;
    std::experimental::optional<size_t> indices;
    size_t material;
    DrawMode mode;

    std::vector<Attributes> targets;
    std::vector<double>     weights;
};

class Mesh {


};

}
