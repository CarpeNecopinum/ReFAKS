#include <gltf/io.hh>
#include <gtest/gtest.h>

#include <gltf/Root.hh>
#include <gltf/Scene.hh>
#include <gltf/Node.hh>
#include <gltf/Camera.hh>
#include <gltf/Buffer.hh>
#include <gltf/BufferView.hh>
#include <gltf/Accessor.hh>

TEST(gltfio, engine)
{
    auto root = gltf::io::readFromFile("../test/gltf/2CylinderEngine.gltf");

    EXPECT_EQ(root.nodes.size(), 82);
    EXPECT_EQ(root.buffers.size(), 1);
    EXPECT_EQ(root.bufferViews.size(), 2);
}
