#pragma once

#include <vector>
#include "core_types.h"
#include <random>

template <typename T = float32, typename S = uint32>
std::vector<T> randomNoise(uint16 dim, bool greyScale = true) {
    std::vector<T> grid(dim*dim*6);

    std::default_random_engine generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<T> distribution(0, 1);

    S curr = 0;

    for (uint16 i = 0; i < dim; i++) {
        for (uint16 j = 0; j < dim; j++) {
            grid[curr++] = i;
            grid[curr++] = j;
            grid[curr++] = 0;

            T r = distribution(generator);
            T g = distribution(generator);
            T b = distribution(generator);

            if (greyScale) {
                T gr = 0.21*r + 0.72*g + 0.07*b;

                grid[curr++] = gr;
                grid[curr++] = gr;
                grid[curr++] = gr;
            } else {
                grid[curr++] = r;
                grid[curr++] = g;
                grid[curr++] = b;
            }
        }
    }

    return grid;
}