#version 330 core

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform float shininess;

// Directional Light
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

// Point Light
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// SpotLight
struct SpotLight {
    vec3 position;
    vec3 direction;


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

uniform vec3 viewPos;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 computePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 normal = texture(normalMap, TexCoord).rgb;

    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);

    // normal in world space
    normal = normalize(TBN * normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
   // vec3 result = computeDirLight(dirLight, norm, viewDir);

    // // phase 2: Point lights
    // for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //     result += computePointLight(pointLights[i], norm, FragPos, viewDir);

    // phase 3: Spot light
    vec3 result = computeSpotLight(spotLight, normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // why -? assuming direction is specified as pointing FROM the light source
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(diffuseMap, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(diffuseMap, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, TexCoord));

    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, TexCoord));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * vec3(texture(diffuseMap, TexCoord));

    // spot
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float cosTheta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((cosTheta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
