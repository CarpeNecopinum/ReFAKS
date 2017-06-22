cmake_minimum_required(VERSION 2.8)

project(ReFAKS)

FILE(GLOB_RECURSE SOURCE_FILES src/*.cc)
FILE(GLOB_RECURSE HEADER_FILES src/*.hh)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
