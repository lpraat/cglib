#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

namespace glp {

    std::string fixPath(std::string toFix) {
        std::vector<char> fixed;

        bool firstFound = false;
        for (uint32 i = 0; i < toFix.length(); i++) {
            if (toFix[i] == '\\') {
                if (!firstFound) {
                    fixed.push_back('/');
                    firstFound = true;
                }
            } else {
                fixed.push_back(toFix[i]);
            }
        }
        fixed.push_back('\0');
        return std::string(fixed.data());
    }

    uint32 textureFromFile(std::string& path, bool gamma = false)
    {
        uint32 textureID;
        glGenTextures(1, &textureID);

        path = fixPath(path);

        int32 width, height, nrComponents;
        ubyte *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}