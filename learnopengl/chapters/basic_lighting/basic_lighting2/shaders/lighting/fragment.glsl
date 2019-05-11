#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 resultColor = (ambient + diffuse) * objectColor;

    FragColor = vec4(resultColor, 1.0);
}