#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Point
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
};

uniform Light light;
uniform Material material;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(- FragPos);

    // Check if object is inside spotlight
    // Assuming spotlight is at the direction of (0.0, 0.0, 1.0)
    float cosTheta = dot(lightDir, vec3(0.0, 0.0, 1.0));
    if (cosTheta > light.cutOff) {
        // diffuse light
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

        // specular light
        vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

        float distance = length(- FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 resultColor = ambient + diffuse + specular;
        FragColor = vec4(resultColor, 1.0);
    } else {
        vec3 resultColor = ambient;
        FragColor = vec4(resultColor, 1.0);
    }

}