#pragma once

#include "vertex.h"
#include "node.h"
#include "model.h"
#include "mat4.h"
#include "vec3.h"
#include "core_types.h"
#include <cmath>

namespace glp {

template <typename T = float32>
class BoundingBox {
public:
    static constexpr float32 vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    static std::vector<T> getUpdatedBounds(const Vec3<T>& position, const T scale) {
        const T x = position.x, y = position.y, z = position.z;

        return {(-0.5f * scale) + x, (0.5f * scale) + x,
                (-0.5f * scale) + y, (0.5f * scale) + y,
                (-0.5f * scale) + z, (0.5f * scale) + z};
    }
};

template <typename T = float32>
class CollisionDetector {
private:
    Model<T>& drone;
    Model<T>& terrain;
    uint32 VBO, VAO;

    uint32 WIDTH = 1001;
    uint32 HEIGHT = 1001;
    uint32 MIN_HEIGHT = 35;

    std::vector<std::vector<T>> height;

public:
    CollisionDetector(Model<T>& drone, Model<T>& terrain) : drone(drone), terrain(terrain) {
        createTerrainGrid();
        setupDebug();
    }

    void createTerrainGrid() {
        std::vector<Node<T>>& nodes = terrain.getNodes();
        height = std::vector<std::vector<T>>(WIDTH, std::vector<T>(HEIGHT, 0.));

        for (uint32 i = 0; i < nodes.size(); i++) {
            for (uint32 j = 0; j < nodes[i].meshes.size(); j++) {
                for (uint32 k = 0; k < nodes[i].meshes[j].vertices.size(); k++) {
                    Vec3<T>& v = nodes[i].meshes[j].vertices[k].Position;
                    int32 x = std::floor(v.x);
                    int32 z = std::floor(v.z + HEIGHT - 1);
                    height[x][z] = std::max(v.y, height[x][z]);
                }
            }
        }

        for (uint32 i = 0; i < WIDTH; i++) {
            for (uint32 j = 0; j < HEIGHT; j++) {
                if (height[i][j] == 0) {
                    height[i][j] = MIN_HEIGHT;
                }
            }
        }
    }

    void setupDebug() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BoundingBox<T>::vertices), BoundingBox<T>::vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float32), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void debug() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    bool hasCollided(const Vec3<T>& dronePosition, T scale) {
        std::vector<T> bounds = BoundingBox<T>::getUpdatedBounds(dronePosition, scale);
        int32 xMin = std::floor(bounds[0]);
        int32 xMax = std::floor(bounds[1]);
        int32 zMin = std::floor(bounds[4]);
        int32 zMax = std::floor(bounds[5]);
        float32 yMin = bounds[2];

        bool inside = xMin > 0 && xMax < WIDTH && zMin > 0 && zMax < HEIGHT;

        if (!inside) return false;

        for (int32 i = xMin; i < xMax; i++) {
            for (int32 j = zMin; j < zMax; j++) {
                if (height[i][j] >= yMin) {
                    return true;
                }
            }
        }
        return false;
    }

};

}; // namespace glp