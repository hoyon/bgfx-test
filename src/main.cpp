#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>

#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/string.h>
#include <bx/math.h>
#include <flecs.h>

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
    float z;
    uint32_t abgr;
};

bgfx::ShaderHandle load_shader(const char* name)
{
    const char* shader_path = "????";

    switch (bgfx::getRendererType())
    {
        case bgfx::RendererType::OpenGL:
            shader_path = "shaders/glsl/";
            break;
        case bgfx::RendererType::Vulkan:
            shader_path = "shaders/spirv/";
            break;
        default:
            BX_CHECK(false, "Unsupported renderer type");
            break;
    }

    char file_path[512];

    bx::strCopy(file_path, BX_COUNTOF(file_path), shader_path);
    bx::strCat(file_path, BX_COUNTOF(file_path), name);
    bx::strCat(file_path, BX_COUNTOF(file_path), ".bin");

    FILE* file_handle = fopen(file_path, "rb");
    fseek(file_handle, 0, SEEK_END);
    uint32_t filelen = ftell(file_handle);
    rewind(file_handle);

    void* file_buffer = malloc((filelen + 1) * sizeof(char));
    fread(file_buffer, filelen, 1, file_handle);
    fclose(file_handle);

    const bgfx::Memory* memory = bgfx::copy(file_buffer, filelen + 1);

    free(file_buffer);

    return bgfx::createShader(memory);
}

bgfx::ProgramHandle load_shader_program(const char* vertex, const char* fragment)
{
    bgfx::ShaderHandle vs = load_shader(vertex);
    bgfx::ShaderHandle fs = load_shader(fragment);

    return bgfx::createProgram(vs, fs, true /* destroy shaders when program is destroyed */);
}

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

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    ecs_world_t* world = ecs_init_w_args(argc, argv);

    bgfx::VertexLayout vertex_layout;
    vertex_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();

    auto red = 0xff0000ff;

    PosColourVertex triangle_vertices[] = {
        {3.f, 3.f, 1.f, red},
        {1.f, 1.f, 1.f, red},
        {5.f, 1.f, 1.f, red}
    };

    const uint16_t triangle_list[] = {0, 1, 2};

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(triangle_vertices, sizeof(triangle_vertices)),
        vertex_layout);

    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(triangle_list, sizeof(triangle_list)));

    bgfx::ProgramHandle program = load_shader_program("vs_triangle", "fs_triangle");

    int counter = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::setViewRect(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        bgfx::touch(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Frame: %d", counter++);

        const bx::Vec3 at = { 0.f, 0.f, 0.f };
        const bx::Vec3 eye = { 0.f, 0.f, -10.f };
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.f, float(SCREEN_WIDTH)/float(SCREEN_HEIGHT), 0.1f, 100.f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, uint16_t(SCREEN_WIDTH), uint16_t(SCREEN_HEIGHT));

        float mtx[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        bgfx::setTransform(mtx);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
