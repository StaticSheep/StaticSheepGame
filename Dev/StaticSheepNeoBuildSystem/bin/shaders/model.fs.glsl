#version 400

in vec3 LitColor;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(LitColor, 1.0);
}