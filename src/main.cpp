#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>

#include <bgfx/platform.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw\n";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "graphics playground", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);

    bgfx::PlatformData platform_data;
    platform_data.nwh = (void*)glfwGetX11Window(window);
    platform_data.ndt = glfwGetX11Display();

    bgfx::Init init;
    init.type = bgfx::RendererType::OpenGL;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.deviceId = 0;
    init.callback = nullptr;
    init.allocator = nullptr;
    init.platformData = platform_data;

    bgfx::init(init);

    bgfx::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::setViewRect(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        bgfx::touch(0);
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
