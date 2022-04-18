#version 410 core

in vec3 v_out_colour;
in vec2 v_out_tex_coord;

uniform sampler2D u_texture;
uniform vec3 u_modifier;

out vec4 f_out_colour;

void main()
{
    f_out_colour = texture(u_texture, v_out_tex_coord) * vec4(u_modifier, 1.0f);
}
