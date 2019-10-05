cmake_minimum_required(VERSION 3.15)
project(graphics-playground)

# don't use legacy opengl on linux
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH $(CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake"))

add_subdirectory(deps)
include(deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake)

find_package(SDL2 REQUIRED)
add_executable(graphics-playground src/main.cpp)

target_link_libraries(graphics-playground PUBLIC bgfx ${SDL2_LIBRARY})
target_include_directories(graphics-playground PUBLIC ${SDL2_INCLUDE_DIRS})