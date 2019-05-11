#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 resultColor;

// Gouraud shading - we calculate the colors in the vertex shader
// In the earlier days of lighting shaders, developers used to implement the Phong lighting model
// in the vertex shader. The advantage of doing lighting in the vertex shader is that it is a lot more efficient
// since there are generally a lot less vertices than fragments, so the (expensive) lighting calculations are
// done less frequently. However, the resulting color value in the vertex shader is the resulting lighting color
// of that vertex only and the color values of the surrounding fragments are then the result of interpolated
// lighting colors. The result was that the lighting was not very realistic unless large amounts of vertices
// were used.
// When the Phong lighting model is implemented in the vertex shader it is called Gouraud shading instead of
// Phong shading. Note that due to the interpolation the lighting looks a bit off.
// The Phong shading gives much smoother lighting results.
void main()
{
    mat4 modelView = view * model;
	gl_Position = projection * modelView * vec4(aPos, 1.0);

    vec3 vPos = vec3(modelView * vec4(aPos, 1.0));
    vec3 LightPos = vec3(view * vec4(lightPos, 1.0));

    // Inversing matrices is a costly operation even for shaders so wherever possible, try to avoid doing inverse
    // operations in shaders since they have to be done on each vertex of your scene.
    // For learning purposes this is fine, but for an efficient application you'll likely want to
    // calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing
    // (just like the model matrix).
    vec3 Normal = mat3(transpose(inverse(modelView))) * aNormal;

    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular light
    float specularStrength = 1.0; // to see better the effect of Gouraud shading
    vec3 viewDir = normalize(-vPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    resultColor = (ambient + diffuse + specular) * objectColor;
}