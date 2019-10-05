#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <iostream>

#include <bgfx/platform.h>
#include <bx/bx.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Failed to initialise SDL\n";
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Graphics playground",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_SysWMinfo window_info;

    SDL_GetWindowWMInfo(window, &window_info);

    bgfx::PlatformData platform_data; 
    platform_data.nwh = (void*)window_info.info.x11.window;
    platform_data.ndt = window_info.info.x11.display;
    std::cerr << "before\n";
    XLockDisplay(window_info.info.x11.display);
    std::cerr << "after\n";

    bgfx::Init init;
    init.type = bgfx::RendererType::OpenGL;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.deviceId = 0;
    init.callback = nullptr;
    init.allocator = nullptr;
    init.platformData = platform_data;

    bgfx::renderFrame();

    bgfx::init(init);
    // bgfx::init();

    bgfx::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

    bgfx::touch(0);

    bgfx::frame();

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
