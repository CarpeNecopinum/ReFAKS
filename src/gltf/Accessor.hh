#pragma once
#include <cstdlib>
#include <vector>
#include <string>

namespace gltf {

struct Accessor {
    enum class ComponentType : uint {
        BYTE = 5120,
        UNSIGNED_BYTE = 5121,
        SHORT = 5122,
        UNSIGNED_SHORT = 5123,
        UNSIGNED_INT = 5124,
        FLOAT = 5126
    };

    size_t bufferView;
    size_t byteOffset;
    ComponentType   componentType;
    size_t count;

    std::vector<double> max;
    std::vector<double> min;
    std::string type;
};

}
