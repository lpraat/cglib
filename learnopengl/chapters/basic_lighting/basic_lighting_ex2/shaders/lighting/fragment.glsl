#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 resultColor = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(resultColor, 1.0);
}