#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

// Directional Light
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
in vec3 DirLightDir;

// Point Light
struct PointLight {
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
in vec3 PointLightPos[NR_POINT_LIGHTS];

// SpotLight
struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;
in vec3 SpotPos;
in vec3 SpotDir;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;

vec3 computeDirLight(DirLight light, vec3 DirLightDir, vec3 normal, vec3 viewDir);
vec3 computePointLight(PointLight light, vec3 PointLightPos, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 computeSpotLight(SpotLight light, vec3 SpotPos, vec3 SpotDir, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    // phase 1: Directional lighting
    vec3 result = computeDirLight(dirLight, DirLightDir, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += computePointLight(pointLights[i], PointLightPos[i], norm, FragPos, viewDir);
    // phase 3: Spot light
    result += computeSpotLight(spotLight, SpotPos, SpotDir, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 computeDirLight(DirLight light, vec3 DirLightDir, vec3 normal, vec3 viewDir)
{
    // why -? assuming direction is specified as pointing FROM the light source
    vec3 lightDir = normalize(-DirLightDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 computePointLight(PointLight light, vec3 PointLightPos, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(PointLightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(PointLightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 computeSpotLight(SpotLight light, vec3 SpotPos, vec3 SpotDir, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(SpotPos - fragPos);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // spot
    float distance = length(SpotPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float cosTheta = dot(lightDir, normalize(SpotDir));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((cosTheta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
