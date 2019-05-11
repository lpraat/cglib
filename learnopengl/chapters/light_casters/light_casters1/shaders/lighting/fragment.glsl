#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightDir; // directional light in view space
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-LightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

    // specular light
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

    vec3 resultColor = ambient + diffuse + specular;

    FragColor = vec4(resultColor, 1.0);
}