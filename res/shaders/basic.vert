#version 410 core

layout (location = 0) in vec3 v_in_pos;
layout (location = 1) in vec3 v_in_colour;
layout (location = 2) in vec2 v_in_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_out_colour;
out vec2 v_out_tex_coord;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(v_in_pos, 1.0);
    v_out_colour = v_in_colour;
    v_out_tex_coord = v_in_tex_coord;
}
