$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

uniform vec4 time;

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 0.0, 1.0));
    v_color0 = vec4(1, abs(sin(time.x)), 0, 1);
}
