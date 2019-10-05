cmake_minimum_required(VERSION 3.15)
project(graphics-playground)

# don't use legacy opengl on linux
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps)
include(deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake)

add_executable(graphics-playground src/main.cpp)
target_link_libraries(graphics-playground PUBLIC bgfx)
