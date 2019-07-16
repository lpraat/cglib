function shaders() {
  // The shader can find the required informations in the following variables:

  //vec3 fs_pos;			// Position of the point in 3D space
  //
  //vec3 LAPos;			// Position of first (or single) light
  //vec3 LADir;			// Direction of first (or single) light
  //float LAConeOut;		// Outer cone (in degree) of the light (if spot)
  //float LAConeIn;		// Inner cone (in percentage of the outher cone) of the light (if spot)
  //float LADecay;		// Decay factor (0, 1 or 2)
  //float LATarget;		// Target distance
  //vec4 LAlightColor;	// color of the first light
  //
  //vec3 LBPos;			// Same as above, but for the second light
  //vec3 LBDir;
  //float LBConeOut;
  //float LBConeIn;
  //float LBDecay;
  //float LBTarget;
  //vec4 LBlightColor;
  //
  //vec3 LCPos;			// Same as above, but for the third one
  //vec3 LCDir;
  //float LCConeOut;
  //float LCConeIn;
  //float LCDecay;
  //float LCTarget;
  //vec4 LClightColor;
  //
  //vec4 ambientLightColor;		// Ambient light color. For hemispheric, this is the color on the top
  //vec4 ambientLightLowColor;	// For hemispheric ambient, this is the bottom color
  //vec3 ADir;					// For hemispheric ambient, this is the up direction
  //
  //float SpecShine;				// specular coefficient for both blinn and phong
  //float DToonTh;				// Threshold for diffuse in a toon shader
  //float SToonTh;				// Threshold for specular in a toon shader
  //
  //vec4 diffColor;				// diffuse color
  //vec4 ambColor;				// material ambient color
  //vec4 specularColor;			// specular color
  //vec4 emit;					// emitted color
  //
  //vec3 normalVec;				// direction of the normal vecotr to the surface
  //vec3 eyedirVec;				// looking direction
  //
  //
  // Final color is returned into:
  //vec4 out_color;

  // Single directional light, Lambert diffuse only: no specular, no ambient, no emission
  var S1 = `
    out_color = LAlightColor * diffColor * clamp(dot(normalize(normalVec), LADir), 0.0, 1.0);
  `;

  // Single point light with decay, Lambert diffuse, Blinn specular, no ambient and no emission
  var S2 = `
    vec3 dir = LAPos - fs_pos;
    float len = length(dir);
    vec3 normalizedDir = dir / len;
    float decay = pow(LATarget / len, LADecay);

    vec4 lightCol = LAlightColor * decay;

    vec4 diffuse = lightCol * diffColor * clamp(dot(normalize(normalVec), normalizedDir), 0.0, 1.0);

    vec3 halfVector = normalizedDir + normalize(eyedirVec);
    vec4 specular = lightCol * specularColor * pow(clamp(dot(normalize(normalVec), normalize(halfVector)), 0.0, 1.0), SpecShine);

    out_color = clamp(diffuse + specular, 0.0, 1.0);
  `;

  // Single directional light, Lambert diffuse, Phong specular, constant ambient and emission
  var S3 = `
    vec4 diffuse = LAlightColor * diffColor * clamp(dot(normalize(normalVec), LADir), 0.0, 1.0);

    vec3 reflected = reflect(-normalize(LADir), normalize(normalVec));
    vec4 specular = LAlightColor * specularColor * pow(clamp(dot(reflected, normalize(eyedirVec)), 0.0, 1.0), SpecShine);

    vec4 ambient = ambientLightColor * ambColor;

    out_color = clamp(ambient + diffuse + specular + emit, 0.0, 1.0);
`;

  // Single spot light (with decay), Lambert diffuse, Blinn specular, no ambient and no emission
  var S4 = `
    vec3 dir = LAPos - fs_pos;
    float len = length(dir);
    vec3 normalizedDir = dir / len;
    float decay = pow(LATarget / len, LADecay);

    float cOut = cos(radians(LAConeOut / 2.0));
    float cIn = cos(radians(LAConeOut * LAConeIn / 2.0));

    vec4 lightCol = LAlightColor * decay * clamp( (dot(normalizedDir, normalize(LADir)) - cOut)/(cIn - cOut), 0.0, 1.0);

    vec4 diffuse = lightCol * diffColor * clamp(dot(normalize(normalVec), LADir), 0.0, 1.0);

    vec3 halfVector = normalizedDir + normalize(eyedirVec);
    vec4 specular = lightCol * specularColor * pow(clamp(dot(normalize(normalVec), normalize(halfVector)), 0.0, 1.0), SpecShine);

    out_color = clamp(diffuse + specular, 0.0, 1.0);
  `;

  // Single directional light, Cartoon diffuse, Cartoon specular, no ambient but emission
  var S5 = `
    vec4 lightCol = LAlightColor;


    vec4 diffuse;
    if (clamp(dot(normalize(normalVec), LADir), 0.0, 1.0) > DToonTh) {
      diffuse = lightCol * diffColor;
    } else {
      diffuse = lightCol * vec4(0.0, 0.0, 0.0, 1.0);
    }

    vec3 reflected = reflect(-normalize(LADir), normalize(normalVec));

    vec4 specular;
    if (clamp(dot(reflected, normalize(eyedirVec)), 0.0, 1.0) > SToonTh) {
      specular = lightCol * specularColor;
    } else {
      specular = lightCol * vec4(0.0, 0.0, 0.0, 1.0);
    }

    out_color = clamp(diffuse + specular + emit, 0.0, 1.0);
  `;

  // Single directional light, no diffuse, phong specular, hemispheric ambient and no emission
  var S6 = `
    vec4 lightCol = LAlightColor;

    vec3 reflected = reflect(-normalize(LADir), normalize(normalVec));
    vec4 specular = lightCol * specularColor * pow(clamp(dot(reflected, normalize(eyedirVec)), 0.0, 1.0), SpecShine);

    vec4 hemispheric = lightCol * ((dot(normalize(normalVec), ADir) + 1.0) / 2.0)*ambientLightColor + ((1.0 - dot(normalize(normalVec), ADir)) / 2.0)*ambientLightLowColor;

    vec4 ambient = hemispheric * ambColor;

    out_color = clamp(ambient + specular, 0.0, 1.0);
  `;

  // Three lights: a directional, a point and a spot. Lambert diffuse, phong specular, constant ambient and no emission
  var S7 = `
    // Directional
    vec4 lightColA = LAlightColor;
    vec4 diffuseA = lightColA * diffColor * clamp(dot(normalize(normalVec), LADir), 0.0, 1.0);

    vec3 reflectedA = reflect(-normalize(LADir), normalize(normalVec));
    vec4 specularA = lightColA * specularColor * pow(clamp(dot(reflectedA, normalize(eyedirVec)), 0.0, 1.0), SpecShine);

    vec4 out_A = clamp(diffuseA + specularA, 0.0, 1.0);

    // Point
    vec3 dirB = LBPos - fs_pos;
    float lenB = length(dirB);
    vec3 normalizedDirB = dirB / lenB;
    float decayB = pow(LBTarget / lenB, LBDecay);

    vec4 lightColB = LBlightColor * decayB;

    vec4 diffuseB = lightColB * diffColor * clamp(dot(normalize(normalVec), normalizedDirB), 0.0, 1.0);

    vec3 reflectedB = reflect(-normalizedDirB, normalize(normalVec));
    vec4 specularB = lightColB * specularColor * pow(clamp(dot(reflectedB, normalize(eyedirVec)), 0.0, 1.0), SpecShine);

    vec4 out_B = clamp(diffuseB + specularB, 0.0, 1.0);

    // Spot
    vec3 dirC = LCPos - fs_pos;
    float lenC = length(dirC);
    vec3 normalizedDirC = dirC / lenC;
    float decayC = pow(LCTarget / lenC, LCDecay);

    float cOut = cos(radians(LCConeOut / 2.0));
    float cIn = cos(radians(LCConeOut * LCConeIn / 2.0));

    vec4 lightColC = LClightColor * decayC * clamp( (dot(normalizedDirC, normalize(LCDir)) - cOut)/(cIn - cOut), 0.0, 1.0);

    vec4 diffuseC = lightColC * diffColor * clamp(dot(normalize(normalVec), LCDir), 0.0, 1.0);

    vec3 reflectedC = reflect(-normalizedDirC, normalize(normalVec));
    vec4 specularC = lightColC * specularColor * pow(clamp(dot(reflectedC, normalize(eyedirVec)), 0.0, 1.0), SpecShine);

    vec4 out_C = clamp(diffuseC + specularC, 0.0, 1.0);


    vec4 ambient = ambientLightColor * ambColor;

    out_color = clamp(out_A + out_B + out_C + ambient, 0.0, 1.0);

  `;


  return [S1, S2, S3, S4, S5, S6, S7];
}

