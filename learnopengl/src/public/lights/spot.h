#pragma once

#include "vec3.h"

namespace glp {

template <typename T = float32>
struct SpotLight {
    Vec3<T> position;
    Vec3<T> direction;

    Vec3<T> ambient;
    Vec3<T> diffuse;
    Vec3<T> specular;

    T constant;
    T linear;
    T quadratic;

    T cutOff;
    T outerCutOff;
};

    // lightingProgram.setVec3("spotLight.ambient", {0.0f, 0.0f, 0.0f});
    // lightingProgram.setVec3("spotLight.diffuse", {1.0f, 1.0f, 1.0f});
    // lightingProgram.setVec3("spotLight.specular", {1.0f, 1.0f, 1.0f});
    // lightingProgram.setFloat("spotLight.constant", 1.0f);
    // lightingProgram.setFloat("spotLight.linear", 0.09);
    // lightingProgram.setFloat("spotLight.quadratic", 0.032);
    // lightingProgram.setFloat("spotLight.cutOff", std::cos(12.5f * glp::toRadians()));
    // lightingProgram.setFloat("spotLight.outerCutOff", std::cos(15.0f * glp::toRadians()));

}; // namespace glp