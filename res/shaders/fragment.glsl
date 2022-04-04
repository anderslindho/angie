#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 modifier;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(modifier, 1.0f);
}
