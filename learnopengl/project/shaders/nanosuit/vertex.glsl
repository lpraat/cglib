#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    mat4 modelView = view * model;
    FragPos = vec3(model * vec4(aPos, 1.0));

    // set this inverse(mat3(model)) as uniform
    //Normal = transpose(inverse(mat3(model))) * aNormal;
    Normal = normalMatrix * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * modelView * vec4(aPos, 1.0);
}