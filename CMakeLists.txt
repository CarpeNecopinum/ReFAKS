cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(ReFAKS)

file(GLOB_RECURSE SOURCE_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES src/*.hh)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
file(GLOB_RECURSE TEST_FILES test/*.cc)

enable_testing()
find_package(GTest REQUIRED)
add_executable(${PROJECT_NAME}_test ${SOURCE_FILES} ${TEST_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_test ${GTEST_BOTH_LIBRARIES})
