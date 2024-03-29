#version 410 core

layout (location = 0) in vec3 v_in_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(v_in_pos, 1.f);
}
