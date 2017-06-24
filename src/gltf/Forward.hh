#pragma once

#include <memory>

namespace gltf {
    struct Node;
    using UniqueNode = std::unique_ptr<Node>;

    struct Scene;
    using UniqueScene = std::unique_ptr<Scene>;

    struct Camera;
    using UniqueCamera = std::unique_ptr<Camera>;

    struct Root;
    using UniqueRoot = std::unique_ptr<Root>;
}
