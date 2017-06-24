#include "io.hh"

#include "Node.hh"
#include "Root.hh"
#include "Scene.hh"
#include "Camera.hh"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#define PICOJSON_USE_INT64
#include <common/picojson.hh>

using picojson::value;
using picojson::object;
using picojson::array;

namespace gltf {
namespace io {

    class gltf_exception : public std::runtime_error {
    public:
        gltf_exception(char const * what) : std::runtime_error(what) {}
    };

    namespace {

        template<class T>
        T get_or_throw(picojson::value const& from, char const* error) {
            if (!from.is<T>()) throw gltf_exception(error);
            return from.get<T>();
        }

        std::vector<size_t> read_indices(picojson::value const& val) {
            std::vector<size_t> result;
            auto arr = get_or_throw<array>(val, "Expected indices to be an array.");

            for (auto idxval : arr) {
                result.push_back(get_or_throw<int64_t>(idxval, "Expected index to be a number."));
            }

            return result;
        }

        UniqueNode readNode(picojson::value const& val) {
            auto node = std::make_unique<Node>();
            auto&& obj = get_or_throw<object>(val, "Expected node to be an object.");

            {
                auto childrenit = obj.find("children");
                if (childrenit != obj.end())
                    node->children = read_indices(childrenit->second);
            }
            {
                auto matrixit = obj.find("matrix");
                if (matrixit != obj.end())
                {
                    auto&& matrix = get_or_throw<array>(matrixit->second, "Expected matrix to be an array.");
                    if (matrix.size() != 16) throw gltf_exception("The matrix should have 16 elements.");

                    for (size_t i = 0; i < 16; i++) {
                        glm::value_ptr(node->matrix)[i] = get_or_throw<double>(matrix[i], "Expected matrix entry to be a number.");
                    }
                }
            }
            return node;
        }

        UniqueScene readScene(picojson::value const& val) {
            auto scene = std::make_unique<Scene>();
            return scene;
        }
    }

    UniqueRoot readFromFile(std::string const& filename) {
        auto root = std::make_unique<Root>();

        auto file = std::ifstream(filename);
        if (!file) throw gltf_exception("could not open file");

        picojson::value jval;
        picojson::parse(jval, file);

        auto&& rootobj = get_or_throw<picojson::object>(jval, "Expected the gltf root to be an object.");

        auto nodesit = rootobj.find("nodes");
        if (nodesit != rootobj.end()) {
            auto&& nodesarr = get_or_throw<picojson::array>(nodesit->second, "Expected nodes to be an array.");
            std::transform(begin(nodesarr), end(nodesarr), back_inserter(root->nodes), readNode);
        }

        return root;
    }
}
}
