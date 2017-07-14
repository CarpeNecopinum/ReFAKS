#include "io.hh"

#include "Root.hh"

#include <experimental/optional>
#include <string>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <typeinfo>

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
        using std::experimental::optional;

        template<class T> struct ReadAs  { using Type = T; };
        template<> struct ReadAs<size_t> { using Type = int64_t; };
        template<> struct ReadAs<uint>   { using Type = int64_t; };

        template<class T>
        T get_or_throw(picojson::value const& from, char const* error) {
            using TR = typename ReadAs<T>::Type;

            if (!from.is<TR>()) throw gltf_exception(error);
            return T(from.get<TR>());
        }

        template<class T>
        struct Reader {
            T operator()(picojson::value const& from)
            {
                return get_or_throw<T>(from, ("Expected a " + std::string(typeid(T).name())).c_str());
            }
        };

        template<class T>
        struct Reader<std::vector<T>> {
            std::vector<T> operator()(picojson::value const& from)
            {
                auto arr = get_or_throw<picojson::array>(from, "Expected an array.");

                std::vector<T> result;
                for (auto&& e : arr) {
                    result.push_back(Reader<T>()(e));
                }
                return result;
            }
        };

        template<class T>
        optional<T> read_optional(picojson::object const& obj, std::string const& index)
        {
            auto it = obj.find(index);
            if (it == end(obj)) return {};

            return Reader<T>()(it->second);
        }

        template<class T>
        std::vector<T> read_vec(picojson::object const& obj, std::string const& index)
        {
            return read_optional<std::vector<T>>(obj, index).value_or(std::vector<T>());
        }

        template<class T>
        T read_mandatory(picojson::object const& obj, std::string const& index)
        {
            auto opt = read_optional<T>(obj, index);
            if (!opt) throw gltf_exception((("missing required attribute " + index).c_str()));
            return opt.value();
        }

        template<>
        struct Reader<Buffer> {
            Buffer operator()(picojson::value const& val) {
                Buffer buffer;
                auto&& obj = get_or_throw<object>(val, "Expected buffer to be an object.");

                buffer.byteLength = read_optional<size_t>(obj, "byteLength").value_or(0);
                buffer.uri        = read_optional<std::string>(obj, "uri");

                return buffer;
            }
        };

        template<>
        struct Reader<BufferView> {
            BufferView operator()(picojson::value const& val) {
                BufferView view;
                auto&& obj = get_or_throw<object>(val, "Expected bufferView to be an object.");

                view.buffer = read_mandatory<size_t>(obj, "buffer");
                view.byteLength = read_optional<size_t>(obj, "byteLength").value_or(0);
                view.byteLength = read_optional<size_t>(obj, "byteOffset").value_or(0);
                view.byteStride = read_optional<size_t>(obj, "byteStride").value_or(0);
                view.target = read_mandatory<uint>(obj, "target");

                return view;
            }
        };

        template<>
        struct Reader<Accessor> {
            Accessor operator()(picojson::value const& val) {
                Accessor acc;
                auto&& obj = get_or_throw<object>(val, "Expected accessor to be an object.");

                acc.bufferView = read_mandatory<size_t>(obj, "bufferView");
                acc.byteOffset = read_optional<size_t>(obj, "byteOffset").value_or(0);
                acc.componentType = Accessor::ComponentType(read_mandatory<size_t>(obj, "byteOffset"));
                acc.count = read_mandatory<size_t>(obj, "count");
                acc.min = read_vec<double>(obj, "min");
                acc.max = read_vec<double>(obj, "max");
                acc.type = read_optional<std::string>(obj, "type").value_or("");

                return acc;
            }
        };

        template<>
        struct Reader<Node> {
            Node operator()(picojson::value const& val){
                Node node;
                auto&& obj = get_or_throw<object>(val, "Expected node to be an object.");

                node.children = read_optional<std::vector<size_t>>(obj, "children").value_or(std::vector<size_t>{});
                {
                    auto matrixit = obj.find("matrix");
                    if (matrixit != obj.end())
                    {
                        auto&& matrix = get_or_throw<array>(matrixit->second, "Expected matrix to be an array.");
                        if (matrix.size() != 16) throw gltf_exception("The matrix should have 16 elements.");

                        for (size_t i = 0; i < 16; i++) {
                            glm::value_ptr(node.matrix)[i] = get_or_throw<double>(matrix[i], "Expected matrix entry to be a number.");
                        }
                    }
                }
                return node;
            }
        };


        template<>
        struct Reader<Scene> {
            Scene operator()(picojson::value const& val) {
                Scene scene;
                return scene;
            }
        };

        template<>
        struct Reader<Camera::Orthographic> {
            Camera::Orthographic operator()(picojson::value const& val) {
                Camera::Orthographic ortho;
                auto&& obj = get_or_throw<picojson::object>(val, "Expected orthographic to be an object.");

                ortho.xmag = read_mandatory<double>(obj, "xmag");
                ortho.ymag = read_mandatory<double>(obj, "ymag");
                ortho.zfar = read_mandatory<double>(obj, "zfar");
                ortho.znear = read_mandatory<double>(obj, "znear");

                return ortho;
            }
        };

        template<>
        struct Reader<Camera::Perspective> {
            Camera::Perspective operator()(picojson::value const& val) {
                Camera::Perspective pers;
                auto&& obj = get_or_throw<picojson::object>(val, "Expected perspective to be an object.");

                pers.aspectRatio = read_optional<double>(obj, "aspectRatio").value_or(1.0);
                pers.yfov = read_mandatory<double>(obj, "yfov");
                pers.zfar = read_optional<double>(obj, "zfar");
                pers.znear = read_mandatory<double>(obj, "znear");

                return pers;
            }
        };

        template<>
        struct Reader<Camera> {
            Camera operator()(picojson::value const& val) {
                Camera cam;
                auto&& obj = get_or_throw<picojson::object>(val, "Expected camera to be an object.");

                cam.name = read_optional<std::string>(obj, "name").value_or("");
                cam.orthographic = read_optional<Camera::Orthographic>(obj, "orthographic");
                cam.perspective = read_optional<Camera::Perspective>(obj, "perspective");
                cam.type = read_optional<std::string>(obj, "type").value_or("");

                return cam;
            }
        };
    }

    Root readFromFile(std::string const& filename) {
        Root root;

        auto file = std::ifstream(filename);
        if (!file) throw gltf_exception("could not open file");

        picojson::value jval;
        picojson::parse(jval, file);

        auto&& rootobj = get_or_throw<picojson::object>(jval, "Expected the gltf root to be an object.");

        root.cameras = read_vec<Camera>(rootobj, "cameras");
        root.nodes = read_vec<Node>(rootobj, "nodes");
        root.scenes = read_vec<Scene>(rootobj, "scenes");
        root.buffers = read_vec<Buffer>(rootobj, "buffers");
        root.bufferViews = read_vec<BufferView>(rootobj, "bufferViews");
        root.accessors = read_vec<Accessor>(rootobj, "accessors");

        return root;
    }
}
}
