cmake_minimum_required(VERSION 3.12)
project(project)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(src src/private src/public ${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})

add_executable(
    project project/project.cpp src/private/glad/glad.c
)

target_link_libraries(
    project ${OPENGL_LIBRARIES} glfw
    /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib # todo ${ASSIMP_LIBRARIES}
)