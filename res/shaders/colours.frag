#version 410 core

in vec3 v_out_pos;
in vec3 v_out_normal;

uniform vec3 u_object_colour;
uniform vec3 u_light_colour;
uniform vec3 u_light_position;
uniform vec3 u_view_position;

out vec4 f_out_colour;

void main()
{
    float ambient_light_strength = .1f;
    float specular_reflection_strength = .5f;

    vec3 ambient_light = ambient_light_strength * u_light_colour;

    vec3 normal = normalize(v_out_normal);
    vec3 light_direction = normalize(u_light_position - v_out_pos);
    float diff = max(dot(normal, light_direction), 0.f);
    vec3 diffuse = diff * u_light_colour;

    vec3 view_direction = normalize(u_view_position - u_light_position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflection_direction), 0.f), 64);

    vec3 result = (ambient_light + diffuse + specular) * u_object_colour;
    f_out_colour = vec4(result, 1.f);
}
