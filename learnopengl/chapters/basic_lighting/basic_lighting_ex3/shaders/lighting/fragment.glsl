#version 330 core

in vec3 resultColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(resultColor, 1.0);
}