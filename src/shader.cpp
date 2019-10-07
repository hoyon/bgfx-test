#include "shader.hpp"

#include <bx/bx.h>
#include <bx/string.h>
#include <bx/math.h>

namespace
{

bgfx::ShaderHandle load_shader(const std::string& shader_filename)
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
    bx::strCat(file_path, BX_COUNTOF(file_path), shader_filename.c_str());
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

} // namespace

bgfx::ProgramHandle load_program(const std::string& vertex_shader, const std::string& fragment_shader)
{
    bgfx::ShaderHandle vs = load_shader(vertex_shader);
    bgfx::ShaderHandle fs = load_shader(fragment_shader);

    return bgfx::createProgram(vs, fs, true /* destroy shaders when program is destroyed */);
}

void free_program(bgfx::ProgramHandle handle)
{
    bgfx::destroy(handle);
}
