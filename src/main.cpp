#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>

#include <bgfx/platform.h>
#include <bx/math.h>
#include <flecs.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

struct PosColourVertex
{
public:
    float x;
    float y;
    uint32_t abgr;
};

int main(int argc, char** argv)
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

    // TODO: check bgfx::getCaps()->homogenousDepth to ensure correct ndc
    // True -> ndc [-1, 1] False -> ndc [0, 1]

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    ecs_world_t* world = ecs_init_w_args(argc, argv);

    bgfx::VertexLayout vertex_layout;
    vertex_layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();

    auto red = 0xff0000ff;

    PosColourVertex triangle_vertices[] = {
        {3.f, 3.f, red},
        {1.f, 1.f, red},
        {5.f, 1.f, red}
    };

    const uint16_t triangle_list[] = {0, 1, 2};

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(triangle_vertices, sizeof(triangle_vertices)),
        vertex_layout);

    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(triangle_list, sizeof(triangle_list)));

    bgfx::UniformHandle time_uniform = bgfx::createUniform("time", bgfx::UniformType::Vec4);

    bgfx::ProgramHandle program = load_program("vs_triangle", "fs_triangle");

    float time = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::setViewRect(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        bgfx::touch(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Graphics playground");

        const auto at2 = glm::vec3{0.f, 0.f, 0.f};
        const auto eye2 = glm::vec3{0.f, 0.f, -10.f};
        // TODO: change to Right handedness
        const auto view = glm::lookAtLH(eye2, at2, glm::vec3{0.f, 1.f, 0.f});

        const auto projection = glm::perspectiveLH(glm::radians(60.f), float(SCREEN_WIDTH)/float(SCREEN_HEIGHT), 0.1f, 100.f);

        bgfx::setViewTransform(0, &view[0][0], &projection[0][0]);
        bgfx::setViewRect(0, 0, 0, uint16_t(SCREEN_WIDTH), uint16_t(SCREEN_HEIGHT));

        const auto transform = glm::identity<glm::mat4>();
        bgfx::setTransform(&transform[0][0]);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::setState(BGFX_STATE_DEFAULT);

        auto value = glm::vec4{time / 20, 0, 0, 0};
        bgfx::setUniform(time_uniform, &value[0]);

        time++;

        bgfx::submit(0, program);

        bgfx::frame();
    }

    free_program(program);
    bgfx::destroy(time_uniform);
    bgfx::destroy(ibh);
    bgfx::destroy(vbh);

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
