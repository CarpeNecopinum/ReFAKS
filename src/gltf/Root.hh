#pragma once
#include "Forward.hh"
#include <vector>

namespace gltf {

class Root {

protected:
    std::vector<Scene> mScenes;
    size_t mDefaultScene;
};

}
