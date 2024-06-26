#version 410 core

layout (location = 0) in vec3 v_in_pos;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texture;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_out_pos;
out vec3 v_out_normal;
out vec2 v_out_texture;

void main()
{
    v_out_pos = vec3(u_model * vec4(v_in_pos, 1.0));
    v_out_normal = v_in_normal;
    v_out_texture = v_in_texture;

    gl_Position = u_projection * u_view * vec4(v_out_pos, 1.f);
}
