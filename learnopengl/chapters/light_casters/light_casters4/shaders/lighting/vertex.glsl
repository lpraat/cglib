#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;

out vec3 SpotPos;
out vec3 SpotDir;
out vec2 TexCoord;

uniform vec3 spotPos;
uniform vec3 spotDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 modelView = view * model;
	gl_Position = projection * modelView * vec4(aPos, 1.0);
    FragPos = vec3(modelView * vec4(aPos, 1.0));

    // Generalized version, we don't know where the spotlight is
    // In case it is at the viewer's position just set SpotPos to 0.0, 0.0, 0.0
    // and SpotDir(forward) to 0.0, 0.0, 1.0
    SpotPos = vec3(view * vec4(spotPos, 1.0));
    SpotDir = vec3(view * vec4(spotDir, 0.0));

    // Inversing matrices is a costly operation even for shaders so wherever possible, try to avoid doing inverse
    // operations in shaders since they have to be done on each vertex of your scene.
    // For learning purposes this is fine, but for an efficient application you'll likely want to
    // calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing
    // (just like the model matrix).
    Normal = mat3(transpose(inverse(modelView))) * aNormal;
    TexCoord = aTexCoord;
}