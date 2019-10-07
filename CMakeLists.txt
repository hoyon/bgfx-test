cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

project(graphics-playground)

# don't use legacy opengl on linux
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH $(CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake"))

add_subdirectory(deps)
include(deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake)

find_package(GLFW REQUIRED)

add_executable(graphics-playground
  src/main.cpp
  shaders/vs_triangle.sc
  shaders/fs_triangle.sc
  )

add_shader(shaders/vs_triangle.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 330)
add_shader(shaders/fs_triangle.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 330)

target_link_libraries(graphics-playground PUBLIC
    bgfx
    ${GLFW_LIBRARIES}
    flecs_shared)

target_include_directories(graphics-playground PUBLIC 
    ${GLFW_INCLUDE_DIRS}
    deps/flecs/include)
