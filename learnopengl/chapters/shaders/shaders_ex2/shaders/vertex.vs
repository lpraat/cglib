#version 330 core

layout (location = 0) in vec3 aPos;

uniform float offset;

void main() {
    gl_Position = vec4(offset + aPos.x, aPos.y, aPos.z, 1.0);
}