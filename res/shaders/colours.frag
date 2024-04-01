#version 410 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_out_pos;
in vec3 v_out_normal;
in vec2 v_out_texture;

uniform vec3 u_view_position;

uniform Material u_material;
uniform Light u_light;

out vec4 f_out_colour;

void main()
{
    // ambient
    vec3 ambient_light = u_light.ambient * vec3(texture(u_material.diffuse, v_out_texture));

    // diffuse
    vec3 normal = normalize(v_out_normal);
    vec3 light_direction = normalize(u_light.position - v_out_pos);
    float diff = max(dot(normal, light_direction), 0.f);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, v_out_texture));

    // specular
    vec3 view_direction = normalize(u_view_position - u_light.position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflection_direction), 0.f), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, v_out_texture));

    vec3 result = ambient_light + diffuse + specular;
    f_out_colour = vec4(result, 1.f);
}
