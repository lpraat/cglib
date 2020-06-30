#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 modelViewProjection;
uniform mat3 normalMatrix;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Compute TBN
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);

    // Reorthogonalization trick to make sure that T is orthogonal to N
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    TexCoord = aTexCoord;

    gl_Position = modelViewProjection * vec4(aPos, 1.0);
}