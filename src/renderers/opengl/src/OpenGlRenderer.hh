#pragma once
#include <glow/fwd.hh>
#include <unordered_map>

namespace gltf {
    struct Root;
}

class OpenGlRenderer {
public:
    void render(gltf::Root const& root);
protected:
    std::unordered_map<std::string, glow::SharedUniformBuffer> mBuffers;
};
