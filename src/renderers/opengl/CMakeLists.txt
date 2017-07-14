cmake_minimum_required(VERSION 2.8)

project(OpenGlRenderer)

add_subdirectory(extern/glow)

file(GLOB_RECURSE SOURCE_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES src/*.hh)

add_library(OpenGlRenderer STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(OpenGlRenderer PROTECTED glow)
