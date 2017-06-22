#pragma once

#include <memory>

namespace gltf {
    class Node;
    using SharedNode = std::shared_ptr<Node>;

    class Scene;
    using SharedScene = std::shared_ptr<Scene>;

    class Camera;
    using SharedCamera = std::shared_ptr<Camera>;

    

}
