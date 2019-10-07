#ifndef SHADER_HPP
#define SHADER_HPP

#include <bgfx/bgfx.h>
#include <string>

bgfx::ProgramHandle load_program(const std::string& vertex_shader, const std::string& fragment_shader);

void free_program(bgfx::ProgramHandle handle);

#endif // SHADER_HPP
