cmake_minimum_required(VERSION 3.12)
project(learnopengl)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(src src/private src/public ${OPENGL_INCLUDE_DIR})

add_executable(
    learnopengl new_main.cpp
)

target_link_libraries(learnopengl ${OPENGL_LIBRARIES} glfw)