#include <iostream>
#include "transformations.h"
#include <vector>
#include "core_types.h"
#include "quat.h"
#include <cmath>
#include "constants.h"

int main() {
    std::vector<float32> v {
        static_cast<float32>(std::cos(30 * pi() / 180)),
        static_cast<float32>(std::sin(30 * pi() / 180)),
        0
        };

    Quat q(90, v);
    q.print();

    std::vector<float32> v1 {1,2,3,1};

    for (auto& el : extractEulerAngles(q.toRotMatrix())) {
        std::cout << "Theta " << el.first * 180/pi() << " " << el.second*180/pi() << std::endl;
    }

    std::cout << "QROTMATRIX" << std::endl;
    for (auto& row : q.toRotMatrix()) {
        for (auto& el : row) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }
    std::vector<float32> m;

    m = dotProduct(q.toRotMatrix(), v1);

    std::cout << "Resulting vector using quat" << std::endl;
    for (auto& el : m) {
        std::cout << el << std::endl;
    }

    auto res = extractEulerAngles(q.toRotMatrix());

    matrix<float32> rot2;

    std::vector<matrix<float32>> composed = {
        rotationZMatrix((res[2].second * 180) / pi()),
        rotationYMatrix((res[0].second * 180) / pi()),
        rotationXMatrix((res[1].second * 180) / pi()),
    };
    rot2 = compose(composed);


    std::cout << "ROT" << std::endl;
    for (auto& row : rot2) {
        for (auto& el : row) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }

    std::vector<float32> m1;

    m1 = dotProduct(rot2, v1);

    std::cout << "Resulting vector using euler" << std::endl;
    for (auto& el : m1) {
        std::cout << el << std::endl;
    }


    auto res2 = extractEulerAnglesAlt(q.toRotMatrix());

    matrix<float32> rot3;

    std::vector<matrix<float32>> composed2 = {
        rotationZMatrix(res2[2] * 180 /pi()),
        rotationYMatrix(res2[1] * 180 / pi()),
        rotationXMatrix(res2[0] * 180 / pi()),
    };
    rot3 = compose(composed2);


    std::cout << "ROT3" << std::endl;
    for (auto& row : rot3) {
        for (auto& el : row) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }

    std::vector<float32> m2;

    m2 = dotProduct(rot3, v1);

    std::cout << "Resulting vector using euler" << std::endl;
    for (auto& el : m2) {
        std::cout << el << std::endl;
    }



}