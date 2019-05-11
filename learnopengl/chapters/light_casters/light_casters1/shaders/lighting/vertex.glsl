#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;

out vec2 TexCoord;
out vec3 LightDir;

uniform vec3 lightDir;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 modelView = view * model;
	gl_Position = projection * modelView * vec4(aPos, 1.0);
    FragPos = vec3(modelView * vec4(aPos, 1.0));

    // Since this is a direction we don't want translation to take effect so we just set the fourth
    // component to 0.0
    // NOTE: this is fixed for every vertex, better to compute it in the cpu once
    LightDir = vec3(view * vec4(lightDir, 0.0)); // light direction in view space

    // Inversing matrices is a costly operation even for shaders so wherever possible, try to avoid doing inverse
    // operations in shaders since they have to be done on each vertex of your scene.
    // For learning purposes this is fine, but for an efficient application you'll likely want to
    // calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing
    // (just like the model matrix).
    Normal = mat3(transpose(inverse(modelView))) * aNormal;
    TexCoord = aTexCoord;
}