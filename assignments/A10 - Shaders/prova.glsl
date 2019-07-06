vec4 lightCol = LAlightColor;

vec3 reflected = normalize(reflect(LADir, normalize(normalVec)));
vec4 specular = lightCol * specularColor * pow(clamp(dot(normalize(reflected), normalize(-eyedirVec)), 0.0, 1.0), SpecShine);

vec4 hemispheric = lightCol * ((dot(normalize(normalVec), LADir) + 1) / 2)*ADir + ((1 - dot(normalize(normalVec), LADir)) / 2)*-ADir

vec4 ambient = ambientLightColor * ambColor;

out_color = clamp(ambient + specular, 0.0, 1.0);