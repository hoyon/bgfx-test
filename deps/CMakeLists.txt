# bgfx
set(BGFX_BUILD_TOOLS    ON CACHE INTERNAL "")
set(BGFX_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BGFX_BUILD_TARGETS  OFF CACHE INTERNAL "")
set(BGFX_OPENGL_VERSION  "33" CACHE STRING "")
add_definitions(-DBGFX_CONFIG_RENDERER_VULKAN=1)
add_subdirectory(bgfx.cmake)

# flecs
add_subdirectory(flecs)
