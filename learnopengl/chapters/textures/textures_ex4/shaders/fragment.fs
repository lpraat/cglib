#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float faceVisibility;

void main() {
    vec2 newFaceCoord = vec2(-TexCoord.x, TexCoord.y);
	FragColor = mix(texture(texture2, newFaceCoord), faceVisibility);
}