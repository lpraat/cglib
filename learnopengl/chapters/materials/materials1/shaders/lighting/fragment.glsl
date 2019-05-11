#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    // ambient light
    vec3 ambient = light.ambient * material.ambient;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular light
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 resultColor = ambient + diffuse + specular;

    FragColor = vec4(resultColor, 1.0);
}