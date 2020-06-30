#pragma once

#include <glad/glad.h>

#include "shader_program.h"
#include "vec3.h"
#include "vec2.h"
#include "core_types.h"
#include "texture_loader.h"

#include "vertex.h"
#include "texture.h"
#include "node.h"
#include "model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace cglib {
template <typename T = float32>
class Mesh {
public:
    uint32 VAO, VBO, EBO;
    std::vector<Vertex<T>> vertices;
    std::vector<uint32> indices;
    std::vector<Texture> textures;

    Node<T>* node;
    Mesh<T>* parent = nullptr;
    std::string name;

public:
    Mesh(Node<T>* node, std::string name, std::vector<Vertex<T>> vertices, std::vector<uint32> indices, std::vector<Texture> textures)
    :
    node(node), name(name), vertices{vertices}, indices{indices}, textures{textures}
    {
        setupMesh();
    }

    ~Mesh() {
        // TODO
        // glDeleteVertexArrays(1, &VAO);
        // glDeleteBuffers(1, &VBO);
        // glDeleteBuffers(1, &EBO);
    }

    /**
     * Setup vertex buffers
     */
    void setupMesh() {
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

        glBindVertexArray(0);
    }

    /**
     * Add a texture. Can be used to add textures after model loading.
     */
    void addTexture(const std::string& type, std::string path) {
        Texture texture {
            TextureLoader::textureFromFile(path),
            type,
            path.substr(path.find_last_of('/')+1, path.size())
        };
        textures.push_back(texture);
    }

    /**
     * Draw the mesh
     */
    void draw(const ShaderProgram& shaderProgram, const Mat4<T>& projection, const Mat4<T>& view) const {
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
            std::string textureType = textures[i].type;
            if (textureType == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (textureType == "texture_specular") {
                number = std::to_string(specularNr++);
            } else if (textureType == "texture_normal") {
                number = std::to_string(normalNr++);
            } else if (textureType == "texture_height") {
                number = std::to_string(heightNr++);
            }

            // Set 2d sampler to corresponding texture unit
            shaderProgram.setInt(textureType + number, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Set model
        shaderProgram.setMat4("model", node->model);
        shaderProgram.setMat3("normalMatrix", node->model.mat3().transposedInverse());
        shaderProgram.setMat4("modelViewProjection", projection.dot(view.dot(node->model)));

        // Draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Reset
        glActiveTexture(GL_TEXTURE0);
    }

    void print() const {
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

    std::string getName() const {
        return name;
    }
};

}; // namespace cglib