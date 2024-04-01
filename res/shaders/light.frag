#version 410 core

uniform vec3 u_light_colour;

out vec4 f_out_colour;

void main()
{
    f_out_colour = vec4(u_light_colour, 1.f);
}
