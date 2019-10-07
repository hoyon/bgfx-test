#include "shader.hpp"

#include <cassert>
#include <fstream>
#include <filesystem>
#include <vector>

namespace
{

std::filesystem::path shader_path(const std::string& filename)
{
    std::filesystem::path base_path = "shaders";

    switch (bgfx::getRendererType())
    {
        case bgfx::RendererType::OpenGL:
            base_path /= "glsl";
            break;
        case bgfx::RendererType::Vulkan:
            base_path /= "spirv";
            break;
        default:
            assert(false && "unsupported shader type");
            break;
    }

    base_path /= (filename + ".bin");
    return base_path;
}

bgfx::ShaderHandle load_shader(const std::string& shader_filename)
{
    auto file_path = shader_path(shader_filename);

    std::ifstream ifs(file_path, std::ios::binary | std::ios::ate);
    auto length = ifs.tellg();

    std::vector<char> bytes(length);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&bytes[0], length);

    const bgfx::Memory* memory = bgfx::copy(&bytes[0], length);

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
