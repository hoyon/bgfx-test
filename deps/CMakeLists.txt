set(BGFX_BUILD_TOOLS    ON CACHE INTERNAL "")
set(BGFX_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BGFX_BUILD_TARGETS  OFF CACHE INTERNAL "")
set(BGFX_OPENGL_VERSION  "33" CACHE STRING "")
add_subdirectory(bgfx.cmake)

add_subdirectory(flecs)
add_subdirectory(glm)
