#pragma once
#include <string>
#include <experimental/optional>
#include <glm/mat4x4.hpp>

namespace gltf {

struct Camera {
    struct Perspective {
        double aspectRatio;
        double yfov;
        double znear;
        std::experimental::optional<double> zfar;
    };

    struct Orthographic {
        double xmag;
        double ymag;
        double zfar;
        double znear;
    };

    std::string name;
    std::string type;

    std::experimental::optional<Perspective> perspective;
    std::experimental::optional<Orthographic> orthographic;

    glm::mat4 matrix() const {

        if (perspective && type == "perspective") {
            auto p = *perspective;
            auto tanfov = std::tan(0.5f * float(p.yfov));

            glm::vec4 col1 = {1.0f / (p.aspectRatio * tanfov), 0, 0, 0};
            glm::vec4 col2 = {0, 1.0f / tanfov, 0, 0};
            glm::vec4 col3, col4;

            if (p.zfar) {
                //Finite perspective projection
                col3 = {0, 0, (*p.zfar + p.znear) / (p.znear - *p.zfar), -1};
                col4 = {0, 0, (2.f * *p.zfar * p.znear) / (p.znear - *p.zfar), 0};
            } else {
                //Infinite perspective projection
                col3 = {0,0,-1.f,-1.f};
                col4 = {0,0,-2.f * p.znear,0};
            }

            return {col1,col2,col3,col4};
        } else if (orthographic && type == "orthographic") {
            auto o = *orthographic;
            glm::vec4 col1 = {1.f / o.xmag, 0, 0, 0};
            glm::vec4 col2 = {0, 1.f / o.ymag, 0, 0};
            glm::vec4 col3 = {0, 0, 2.f / (o.znear - o.zfar), 0};
            glm::vec4 col4 = {0, 0, (o.zfar + o.znear) / (o.znear - o.zfar), 1.f};

            return {col1, col2, col3, col4};
        }

        return {};
    }
};

}
