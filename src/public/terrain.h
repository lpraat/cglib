#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core_types.h"
#include "vector"
#include "vec3.h"
#include "perlin.h"

namespace cglib {

class Terrain {
private:
    uint32 VAO, VBO, EBO;
    std::vector<float32> vertices;
    std::vector<uint32> indices;
    std::vector<Vec3<float32>> normals;

    uint32 GridSize;
    const uint32 TileSize;
    std::vector<std::vector<float32>> height;

    // TODO remove and use textures
    uint32 colorVBO;
    std::vector<float32> colors;



public:
    Terrain(uint32 size, uint32 tileSize) : GridSize(size), TileSize(tileSize) {
        createGrid();
        createIndices();
        setup();
    }

    void calculateNormals() {
        // TODO - first refactor using Vertex!!
    }

    void createGrid() {
        vertices = std::vector<float32>((GridSize * GridSize / TileSize*TileSize) * 3);
        colors = std::vector<float32>((GridSize * GridSize / TileSize*TileSize) * 3);
        height = std::vector(GridSize, std::vector<float32>(GridSize));

        uint64 curr = 0;
        uint64 colorCurr = 0;

        for (uint16 i = 0; i < GridSize; i += TileSize) {
            for (uint16 j = 0; j < GridSize; j += TileSize) {
                float32 generatedHeight = 10*cglib::PerlinNoise<float32>::noiseOctaves(static_cast<float32>(i) / GridSize, static_cast<float32>(j) / GridSize, 0, 3, 0.3, 4);
                height[i][j] = generatedHeight;
                vertices[curr++] = i;
                vertices[curr++] = generatedHeight;
                vertices[curr++] = j;

                if (generatedHeight <= 4*2) {
                    colors[colorCurr++] = 0.0f;
                    colors[colorCurr++] = 1.0f;
                    colors[colorCurr++] = 1.0f;
                } else {
                    colors[colorCurr++] = 0.0f;
                    colors[colorCurr++] = 1.0f;
                    colors[colorCurr++] = 0.0f;
                }
            }
        }
    }

    float32 getHeight(uint32 x, uint32 z) {
        if (x < 0 || x >= GridSize || z < 0 || z >= GridSize) {
            return 0;
        }
        return height[x][z];
    }

    void createIndices() {
        uint64 j = 0;
        for (uint64 i = 0; i < (GridSize/TileSize) - 1; i++) {
            while (true) {
                indices.push_back(j); indices.push_back(j+1); indices.push_back(j + (GridSize/TileSize));
                indices.push_back(j+1); indices.push_back(j+ (GridSize/TileSize) +1); indices.push_back(j+ (GridSize/TileSize));

                if (j == (GridSize/TileSize) *(i+1) - 2) {
                    j+=2;
                    break;
                } else {
                    j++;
                }
            }
        }
    }

    void setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &colorVBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float32), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float32), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float32), colors.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float32), (void*)0);
        glEnableVertexAttribArray(1);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


};

}; // namespace cglib