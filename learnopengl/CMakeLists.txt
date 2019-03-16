cmake_minimum_required(VERSION 3.12)
project(learnopengl)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(private public ${OPENGL_INCLUDE_DIRS})

add_executable(
    learnopengl main.cpp
    private/glad/glad.c
)

target_link_libraries(learnopengl ${OPENGL_LIBRARIES} glfw)