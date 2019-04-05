#pragma once

#include <glad/glad.h>

#include "core_types.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <iostream>
#include "mat4.h"

class ShaderProgram {
private:
    uint32 id;

public:
    explicit ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexShaderCodeStr;
        std::string fragmentShaderCodeStr;
        std::stringstream vertexShaderStream;
        std::stringstream fragmentShaderStream;

        std::ifstream vertexShaderFile(vertexPath);

        if (!vertexShaderFile) {
            std::cout << "Error loading Vertex Shader" << std::endl;
        }

        std::ifstream fragmentShaderFile(fragmentPath);
        if (!fragmentShaderFile) {
            std::cout << "Error loading Fragment shader" << std::endl;
        }

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderCodeStr = vertexShaderStream.str();
        fragmentShaderCodeStr = fragmentShaderStream.str();

        uint32 vertexShader, fragmentShader;
        const char* vertexShaderCode;
        const char* fragmentShaderCode;

        vertexShaderCode = vertexShaderCodeStr.c_str();
        fragmentShaderCode = fragmentShaderCodeStr.c_str();

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
        glCompileShader(vertexShader);
        checkShader(vertexShader, "Vertex");

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
        glCompileShader(fragmentShader);
        checkShader(fragmentShader, "Fragment");

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        checkProgram();

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~ShaderProgram() {
        glDeleteProgram(id);
    }

    uint32 getId() const {
        return id;
    }

    void use() const {
        glUseProgram(id);
    }

    void checkShader(uint32 shaderId, const std::string& type) const {
        int32 success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success) {
            int32 infoLogLen;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen);

            char infoLog[infoLogLen];
            glGetShaderInfoLog(shaderId, infoLogLen, nullptr, infoLog);
            std::cout << "Error compiling " << type <<  " shader.\n" << infoLog << std::endl;
        }
    }

    void checkProgram() const {
        int32 success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success) {
            int32 infoLogLen;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLen);

            char infoLog[infoLogLen];
            glGetProgramInfoLog(id, infoLogLen, nullptr, infoLog);
            std::cout << "Error compiling shader program.\n" << infoLog << std::endl;
        }
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int32)value);
    }

    void setInt(const std::string& name, int32 value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string& name, float32 value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setMat4(const std::string& name, glp::Mat4<float32>& m4) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_TRUE, m4.getPtr());
    }

};