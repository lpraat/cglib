#pragma once

#include <glad/glad.h>

#include "shader_program.h"
#include "vec3.h"
#include "vec2.h"
#include "core_types.h"
#include "texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace glp {

template <typename T = float32>
struct Vertex {
    Vec3<T> Position;
    Vec3<T> Normal;
    Vec2<T> TexCoords;
    Vec3<T> Tangent;
    Vec3<T> Bitangent;
};

struct Texture {
    uint32 id;
    std::string type;
    std::string path;
};

template <typename T = float32>
class Mesh {
public:
    uint32 VAO, VBO, EBO;
    std::vector<Vertex<T>> vertices;
    std::vector<uint32> indices;
    std::vector<Texture> textures;

    Mesh<T>* parent = nullptr;
    std::string name;

public:

    Mesh(std::string name, std::vector<Vertex<T>> vertices, std::vector<uint32> indices, std::vector<Texture> textures)
    :
    name(name), vertices{vertices}, indices{indices}, textures{textures}
    {
        setupMesh();
    }

    ~Mesh() {
        // glDeleteVertexArrays(1, &VAO);
        // glDeleteBuffers(1, &VBO);
        // glDeleteBuffers(1, &EBO);
    }

    /**
     * Setup vertex buffers
     */
    void setupMesh()
    {
        // Create buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex<T>), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (void*)0);

        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (void*)offsetof(Vertex<T>, Normal));

        // Texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (void*)offsetof(Vertex<T>, TexCoords));

        // Tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (void*)offsetof(Vertex<T>, Tangent));

        // Bitangents
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (void*)offsetof(Vertex<T>, Bitangent));

        glBindVertexArray(0);
    }

    /**
     * Add a texture. Can be used to add textures after model loading.
     */
    void addTexture(const std::string& type, const std::string& path) {
        Texture texture {
            textureFromFile(path),
            type,
            path.substr(path.find_last_of('/')+1, path.size())
        };
        textures.push_back(texture);
    }

    /**
     * Draw the mesh
     */
    void draw(const ShaderProgram& shaderProgram)
    {
        // Bind textures
        uint32 diffuseNr  = 1;
        uint32 specularNr = 1;
        uint32 normalNr   = 1;
        uint32 heightNr   = 1;

        shaderProgram.use();
        for(uint32 i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
            else if (name == "texture_normal")
				number = std::to_string(normalNr++);
            else if (name == "texture_height")
			    number = std::to_string(heightNr++);

            // Set 2d sampler to corresponding texture unit
            shaderProgram.setInt((name + number).c_str(), i);
            // glUniform1i(glGetUniformLocation(shaderProgram.getId(), (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Reset
        glActiveTexture(GL_TEXTURE0);
    }

    void print() {
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << "Mesh: " << name << std::endl;

        if (parent != nullptr) {
            std::cout << "Parent: " << parent->getName() << std::endl;
        }
        std::cout << "Vertices: " << vertices.size() << " ";
        std::cout << "Indices: " << indices.size() << std::endl;

        std::cout << "Texture size " << textures.size() << std::endl;
        for (uint32 i = 0; i < textures.size(); i++) {
            if (i == 0) {
                std::cout << "Textures: " << std::endl;
            }
            std::cout << "Path: " << textures[i].path << " ";
            std::cout << "Type: " << textures[i].type << std::endl;
        }
    }

    std::string getName() {
        return name;
    }
};

}; // namespace glp