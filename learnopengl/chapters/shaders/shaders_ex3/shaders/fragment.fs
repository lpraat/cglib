#version 330 core

in vec4 colorVertex;
out vec4 FragColor;

void main() {
    FragColor = colorVertex;
}