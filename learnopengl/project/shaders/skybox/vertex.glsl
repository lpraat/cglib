#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewProjection;

void main()
{
    TexCoords = aPos;
    vec4 pos = viewProjection * vec4(aPos, 1.0);

    // This way z will be always at the maximum depth in normalized screen coordinates (/w)
    gl_Position = pos.xyww;
}