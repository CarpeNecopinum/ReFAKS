cmake_minimum_required(VERSION 2.8)

project(VulkanRenderer)

#add_subdirectory(extern/glow)

file(GLOB_RECURSE SOURCE_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES src/*.hh)

add_library(VulkanRenderer STATIC ${SOURCE_FILES} ${HEADER_FILES})
