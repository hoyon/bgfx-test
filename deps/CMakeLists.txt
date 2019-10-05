# bgfx
set(BGFX_BUILD_TOOLS    ON CACHE INTERNAL "")
set(BGFX_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BGFX_BUILD_TARGETS  OFF CACHE INTERNAL "")
add_subdirectory(bgfx.cmake)

# flecs
add_subdirectory(flecs)
